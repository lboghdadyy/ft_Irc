#include "bot.hpp"

void    initFacts(std::vector<std::string> &funFacts)
{
    funFacts.push_back("Bananas are berries, but strawberries are not.");
    funFacts.push_back("Honey never spoils. Edible honey has been found in ancient Egyptian tombs.");
    funFacts.push_back("Octopuses have three hearts and blue blood.");
    funFacts.push_back("A day on Venus is longer than a year on Venus.");
    funFacts.push_back("Sharks existed before trees.");
    funFacts.push_back("The Eiffel Tower grows about 15 centimeters taller in summer due to heat expansion.");
    funFacts.push_back("Wombats produce cube-shaped droppings.");
    funFacts.push_back("There are more possible chess games than atoms in the observable universe.");
    funFacts.push_back("An astronaut can grow up to 5 centimeters taller in space.");
    funFacts.push_back("The fingerprints of a koala are almost indistinguishable from those of a human.");
    funFacts.push_back("A group of flamingos is called a flamboyance.");
    funFacts.push_back("The shortest war in history lasted only 38 minutes.");
    funFacts.push_back("Scotland's national animal is the unicorn.");
    funFacts.push_back("Some turtles can breathe through their backsides.");
    funFacts.push_back("The inventor of the Pringles can is buried in one.");
    funFacts.push_back("Cows have best friends and become stressed when separated.");
    funFacts.push_back("The first computer bug was an actual moth trapped in a computer.");
    funFacts.push_back("Water can boil and freeze at the same time under the right conditions.");
    funFacts.push_back("A cloud can weigh over a million pounds.");
    funFacts.push_back("The human brain uses about 20% of the body's total energy.");
    funFacts.push_back("A shrimp's heart is located in its head.");
    funFacts.push_back("Sloths can hold their breath longer than dolphins.");
    funFacts.push_back("The world's largest desert is Antarctica.");
    funFacts.push_back("There are more stars in the universe than grains of sand on Earth.");
    funFacts.push_back("A bolt of lightning is about five times hotter than the surface of the Sun.");
    funFacts.push_back("The first oranges weren't orange—they were green.");
    funFacts.push_back("Cats can't taste sweetness.");
    funFacts.push_back("The Moon is slowly drifting away from Earth by about 3.8 centimeters each year.");
    funFacts.push_back("Sea otters hold hands while sleeping so they don't drift apart.");
    funFacts.push_back("The inventor of the microwave discovered it after a chocolate bar melted in his pocket.");
    funFacts.push_back("Polar bears have black skin beneath their white fur.");
    funFacts.push_back("The longest English word without a vowel is 'rhythms'.");
    funFacts.push_back("The dot over the letters 'i' and 'j' is called a tittle.");
    funFacts.push_back("Your stomach gets a new lining every few days.");
    funFacts.push_back("The average person walks the equivalent of five times around Earth in their lifetime.");
    funFacts.push_back("The heart of a blue whale is about the size of a small car.");
    funFacts.push_back("Crows can recognize human faces.");
    funFacts.push_back("Some bamboo species can grow over 90 centimeters in a single day.");
    funFacts.push_back("There are more possible ways to shuffle a deck of cards than there are atoms on Earth.");
    funFacts.push_back("A snail can sleep for up to three years.");
    funFacts.push_back("The Eiffel Tower was originally intended to be dismantled after 20 years.");
    funFacts.push_back("Jellyfish have existed longer than dinosaurs.");
    funFacts.push_back("A group of crows is called a murder.");
    funFacts.push_back("The unicorn is the national animal of Scotland.");
    funFacts.push_back("Pineapples take about two years to grow.");
    funFacts.push_back("The first alarm clock could only ring at 4 a.m.");
    funFacts.push_back("Butterflies taste with their feet.");
    funFacts.push_back("The shortest commercial flight in the world lasts less than two minutes.");
    funFacts.push_back("An ostrich's eye is bigger than its brain.");
    funFacts.push_back("The longest hiccuping spree lasted 68 years.");
    funFacts.push_back("The tongue is the only muscle attached at one end.");
    funFacts.push_back("Koalas sleep up to 22 hours a day.");
    funFacts.push_back("Some frogs can freeze solid during winter and thaw back to life.");
    funFacts.push_back("The first webcam watched a coffee pot.");
    funFacts.push_back("The Sun contains about 99.8% of the Solar System's mass.");
    funFacts.push_back("A crocodile cannot stick its tongue out.");
    funFacts.push_back("Humans are the only animals that blush.");
    funFacts.push_back("The shortest complete sentence in English is 'I am.'");
    funFacts.push_back("A day on Mercury lasts about 176 Earth days.");
    funFacts.push_back("The world's oldest known pet cat lived over 9,000 years ago.");
    funFacts.push_back("Penguins propose to their mates with pebbles.");
    funFacts.push_back("The average cloud travels at about 30–40 km/h.");
    funFacts.push_back("An apple, potato, and onion all taste the same if you eat them with your nose plugged.");
    funFacts.push_back("The human nose can remember about 50,000 different scents.");
    funFacts.push_back("More people are bitten by New Yorkers each year than by sharks.");
    funFacts.push_back("A giraffe has the same number of neck bones as a human: seven.");
    funFacts.push_back("The inventor of LEGO never imagined the bricks would become one of the world's most popular toys.");
    funFacts.push_back("An octopus can squeeze through any hole larger than its beak.");
    funFacts.push_back("The Earth is not a perfect sphere—it's slightly flattened at the poles.");
    funFacts.push_back("The word 'robot' comes from a Czech word meaning 'forced labor'.");
}

Bot::Bot(std::string &h, int p, std::string &pass, std::string &n, std::string &c)
{
    host = h;
    port = p;
    password = pass;
    nick = n;
    channel = c;
    user = nick;
    initFacts(funFacts);
}

Bot::Bot(const Bot &x)
{
    *this = x;
}

Bot& Bot::operator=(const Bot &x)
{
    (void)x;
    return *this;
}

bool    Bot::sendline(const std::string &x)
{
    std::string buf = x + "\r\n";
    ssize_t s = send(sfd, buf.c_str(), buf.size(), 0);
    if (s < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true;
        std::cerr<<"Bot : send() failed\n";
        return false;
    }
    return true;
}

void    Bot::auth()
{
    sendline("PASS " + password);
    sendline("NICK " + nick);
    sendline("USER " + user + " 0 * :" + user + " bot");
    sendline("JOIN " + channel);
}

void    Bot::sendFunFact(void)
{
    std::srand(std::time(0));
    int randomFact = std::rand() % this->funFacts.size();
    
    sendline("PRIVMSG " + channel + " :" + this->funFacts.at(randomFact));
}

void    Bot::parseBuffer(const char *buffer)
{
    std::string         buff(buffer);
    std::stringstream   _stream(buff);
    std::string         string;
    std::string         message;
    int                 index = 0;
    bool                himAgain = false;
    static std::vector<std::string>  nicks;
    
    while (index < 3)
    {
        _stream >> message;
        if (index == 0)
        {
            if (std::find(nicks.begin(), nicks.end(), message) == nicks.end())
                nicks.push_back(message);
            else 
                himAgain = 1;
        }
        index++;
    }
    _stream >> string;
    if (string == ":!fact")
    {
        if (himAgain)
        {
            sendline("PRIVMSG " + channel + " :YOU AGAIN 🤨​\n");
            sleep(1);
        }
        sendline("PRIVMSG " + channel + " :hmmmmmmmmmmmm ... let me think ​🤔​");
        sleep(1);
        sleep(1);
        sendline("PRIVMSG " + channel + " :found it 💡");
        sleep(1);
        sendFunFact();
    }
}

bool    Bot::loop()
{

    std::cout<<"Bot is running ...\n";
    std::cout<<"Bot commands: !fact, !tiki\n";

    char buff[4096];
    while (1)
    {
        ssize_t r = recv(sfd, buff, sizeof(buff), 0);
        if (r == 0)
        {
            std::cerr << "Bot : server closed connection\n";
            return false;
        }
        if (r < 0)
        {
            std::cerr << "Bot : recv() failed\n";
            return false;
        }
        recvBuff.append(buff, r);
        std::cout << buff << std::endl;
        parseBuffer(buff);
    }
    return (true);
}

Bot::Bot() {

}

bool Bot::run()
{
    struct sockaddr_in x;
    std::memset(&x, 0, sizeof(x));
    x.sin_family = AF_INET;
    x.sin_port = htons(port);
    x.sin_addr.s_addr = inet_addr(host.c_str());
    if (x.sin_addr.s_addr == INADDR_NONE)
    {
        std::cerr<<"Bot : host must be a valid IPv4 address\n";
        return false;
    }
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        std::cerr<<"Bot : socket failed\n";
        return false;
    }
    if (connect(sfd, (struct sockaddr *)&x, sizeof(x)) < 0)
    {
        std::cerr<<"Bot : connect() failed\n";
        return false;
    }
    std::cout << "Bot: connected to " <<host<<std::endl;
    auth();
    return loop();
}

Bot::~Bot()
{
    if (sfd >= 0)
        close(sfd);
    std::cout<<"Bot disconnected\n";
}