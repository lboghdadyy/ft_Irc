#!/usr/bin/env bash

set -u -o pipefail

HOST="127.0.0.1"
PORT="${1:-6667}"
PASSWD="${2:-mypassword}"
SERVER_BIN="${3:-./ircserv}"

if [[ ! -x "$SERVER_BIN" ]]; then
  echo "Server binary not found or not executable: $SERVER_BIN"
  echo "Build first with: make"
  exit 1
fi

if ! command -v nc >/dev/null 2>&1; then
  echo "Missing dependency: nc (netcat)"
  exit 1
fi

PASS_COUNT=0
FAIL_COUNT=0

declare -A C_IN
declare -A C_PID
declare -A C_FIFO
declare -A C_OUTFILE

SERVER_PID=""

pass() {
  echo "[PASS] $*"
  PASS_COUNT=$((PASS_COUNT + 1))
}

fail() {
  echo "[FAIL] $*"
  FAIL_COUNT=$((FAIL_COUNT + 1))
}

start_server() {
  "$SERVER_BIN" "$PORT" "$PASSWD" >/tmp/ft_irc_test_server.log 2>&1 &
  SERVER_PID=$!
  sleep 0.3

  if ! kill -0 "$SERVER_PID" >/dev/null 2>&1; then
    echo "Could not start server. Check /tmp/ft_irc_test_server.log"
    exit 1
  fi
}

start_client() {
  local name="$1"
  local fifo
  local outfile
  local wfd

  fifo="$(mktemp -u "/tmp/ft_irc_${name}_in.XXXX")"
  outfile="$(mktemp "/tmp/ft_irc_${name}_out.XXXX")"
  mkfifo "$fifo"

  if command -v stdbuf >/dev/null 2>&1; then
    stdbuf -o0 nc "$HOST" "$PORT" <"$fifo" >"$outfile" 2>&1 &
  else
    nc "$HOST" "$PORT" <"$fifo" >"$outfile" 2>&1 &
  fi
  C_PID[$name]=$!

  exec {wfd}>"$fifo"
  C_IN[$name]="$wfd"
  C_FIFO[$name]="$fifo"
  C_OUTFILE[$name]="$outfile"
}

send_cmd() {
  local name="$1"
  local line="$2"
  printf '%s\r\n' "$line" >&"${C_IN[$name]}"
}

expect_contains() {
  local name="$1"
  local needle="$2"
  local timeout="${3:-3}"
  local label="${4:-$needle}"
  local end=$((SECONDS + timeout))

  while (( SECONDS <= end )); do
    if grep -Fq "$needle" "${C_OUTFILE[$name]}"; then
      pass "$label"
      return 0
    fi
    sleep 0.1
  done

  fail "$label"
  echo "  expected in client $name output: $needle"
  echo "  output file: ${C_OUTFILE[$name]}"
  tail -n 20 "${C_OUTFILE[$name]}" 2>/dev/null | sed 's/^/  | /'
  return 1
}

expect_disconnect() {
  local name="$1"
  local timeout="${2:-4}"
  local label="${3:-disconnect $name}"
  local end=$((SECONDS + timeout))

  while (( SECONDS <= end )); do
    if ! kill -0 "${C_PID[$name]}" >/dev/null 2>&1; then
      pass "$label"
      return 0
    fi
    sleep 0.1
  done

  fail "$label"
  return 1
}

cleanup() {
  local name
  for name in "${!C_IN[@]}"; do
    eval "exec ${C_IN[$name]}>&-" 2>/dev/null || true
  done
  for name in "${!C_PID[@]}"; do
    kill "${C_PID[$name]}" >/dev/null 2>&1 || true
  done
  for name in "${!C_FIFO[@]}"; do
    rm -f "${C_FIFO[$name]}" >/dev/null 2>&1 || true
  done
  for name in "${!C_OUTFILE[@]}"; do
    rm -f "${C_OUTFILE[$name]}" >/dev/null 2>&1 || true
  done

  if [[ -n "$SERVER_PID" ]]; then
    kill "$SERVER_PID" >/dev/null 2>&1 || true
    wait "$SERVER_PID" 2>/dev/null || true
  fi
}

trap cleanup EXIT

register_client() {
  local name="$1"
  local nick="$2"
  local user="$3"

  send_cmd "$name" "PASS $PASSWD"
  send_cmd "$name" "NICK $nick"
  send_cmd "$name" "USER $user 0 * :$user real"
  expect_contains "$name" " 001 $nick " 4 "REGISTER $nick"
}

echo "== ft_irc command test =="
echo "Server: $SERVER_BIN  Host: $HOST  Port: $PORT"

start_server

start_client "A"
start_client "B"
start_client "C"

send_cmd "A" "JOIN #precheck"
expect_contains "A" " 451 * :You have not registered" 2 "NOT REGISTERED guard"

register_client "A" "alpha" "ualpha"
register_client "B" "beta" "ubeta"
register_client "C" "gamma" "ugamma"

send_cmd "A" "JOIN #room"
expect_contains "A" "JOIN #room" 3 "JOIN #room by alpha"

send_cmd "B" "JOIN #room"
expect_contains "B" "JOIN #room" 3 "JOIN #room by beta"

send_cmd "A" "PRIVMSG #room :hello channel"
expect_contains "B" "PRIVMSG #room :hello channel" 3 "PRIVMSG channel"

send_cmd "B" "PRIVMSG alpha :hello alpha"
expect_contains "A" "PRIVMSG alpha :hello alpha" 3 "PRIVMSG private"

send_cmd "A" "TOPIC #room :topic from alpha"
expect_contains "B" "TOPIC #room :topic from alpha" 3 "TOPIC set"

send_cmd "A" "MODE #room +t"
expect_contains "A" "MODE #room +t" 3 "MODE +t"

send_cmd "A" "MODE #room +o beta"
expect_contains "B" "MODE #room +o beta" 3 "MODE +o"

send_cmd "B" "KICK #room alpha :out"
expect_contains "A" "KICK #room alpha :out" 3 "KICK"

send_cmd "A" "JOIN #invite"
expect_contains "A" "JOIN #invite" 3 "JOIN #invite by alpha"

send_cmd "A" "MODE #invite +i"
expect_contains "A" "MODE #invite +i" 3 "MODE +i"

send_cmd "C" "JOIN #invite"
expect_contains "C" " 473 gamma #invite :Cannot join channel (+i)" 3 "INVITE-ONLY block"

send_cmd "A" "INVITE gamma #invite"
expect_contains "A" " 341 alpha gamma #invite" 3 "INVITE reply"
expect_contains "C" "INVITE gamma :#invite" 3 "INVITE delivered"

send_cmd "C" "JOIN #invite"
expect_contains "C" "JOIN #invite" 3 "JOIN invited user"

send_cmd "A" "MODE #invite +k secret"
expect_contains "A" "MODE #invite +k secret" 3 "MODE +k"

send_cmd "A" "MODE #invite +l 3"
expect_contains "A" "MODE #invite +l 3" 3 "MODE +l"

send_cmd "A" "PART #invite :bye"
expect_contains "A" "PART #invite :bye" 3 "PART"

send_cmd "B" "NICK beta2"
expect_contains "B" "NICK :beta2" 3 "NICK change"

send_cmd "C" "QUIT :leaving"
expect_contains "C" "ERROR :Closing Link:" 3 "QUIT"

echo
echo "== Summary =="
echo "Passed: $PASS_COUNT"
echo "Failed: $FAIL_COUNT"

if (( FAIL_COUNT > 0 )); then
  exit 1
fi

exit 0
