#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client {
    private:
        int         fd;
        std::string nick;
        std::string user;
        std::string host;
        bool        registered;
        std::string readBuffer;
    public:
        // sbouaa Part + OCF 
        Client(int fd, const std::string &hostname);
        ~Client();
        // oussama ay7taj had l getters
        int         getFd()       const;  // the socket file descriptor
        std::string getNick()     const;  // "FIESKA"
        std::string getUser()     const;  // "FIESKA" (username, may differ)
        std::string getHost()     const;  // "192.168.1.1"
        bool        isRegistered() const; // true only after PASS+NICK+USER done

        // Prefix for IRC messages: "nick!user@host"
        std::string getPrefix()   const; // an7tajeha (oussama) bach n buildi wa7ed replay like ":salah!salah@host JOIN #general"

        void setNick(const std::string &nick);
        void setUser(const std::string &user);
        void setRegistered(bool val);
        /*
        // dber rassek 
        void        appendToBuffer(const std::string &data);
        std::string getBuffer()   const;
        void        clearBuffer();
        */
};

#endif