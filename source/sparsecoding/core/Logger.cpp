#include "Logger.h"


////////////////////////////////////////////////////////////////////////////////
Logger::Logger(const int prio) :
    prio_(prio)
{}


////////////////////////////////////////////////////////////////////////////////
Logger::Logger(const string& msg, const int prio) :
    prio_(prio)
{
    push(msg);
}


////////////////////////////////////////////////////////////////////////////////
Logger::~Logger() { 
    pop(); 
}


////////////////////////////////////////////////////////////////////////////////
const string& Logger::getConfirmation()
{
    static const string defaultConfirmation("OK");

    if (confType_ == "default") {
        const int id = rand() % confirmations_.size();
        return confirmations_[id];
    } else if (confType_ == "girls") {
        const int id = rand() % confirmationsGirl_.size();
        return confirmationsGirl_[id];
    } else if (confType_ == "doge") {
        const int id = rand() % confirmationsDoge_.size();
        return confirmationsDoge_[id];
    } else {
        return defaultConfirmation;
    }
}


////////////////////////////////////////////////////////////////////////////////
void Logger::setMaxLevel(const int level) {
    maxLevel_ = level;
}


////////////////////////////////////////////////////////////////////////////////
const int Logger::getCurrentLevel() {
    return (int)parents_.size();
}


////////////////////////////////////////////////////////////////////////////////
void Logger::push(const string& parent)
{    
    if (active_)
    {
        if (maxLevel_ < 0 || getCurrentLevel() < maxLevel_) 
        {
            if (!interrupts_.empty())
            {
                if (!interrupts_.top()) {
                    cout << endl;
                    interrupts_.top() = true;
                }
            }

            indent();

            if (parents_.size() > 0) {
                cout << "|-+ ";
            } 

            cout << parent << " ... ";
        }
    }

    Timer timer;
    timer.start();

    parents_.push(parent);
    timers_.push(timer);
    interrupts_.push(false);
}


////////////////////////////////////////////////////////////////////////////////
void Logger::pop(bool success)
{ 
    if (!parents_.empty())
    {
        Timer timer = timers_.top();
        const string parent = parents_.top();
        const bool interrupt = interrupts_.top();

        timers_.pop();
        parents_.pop();
        interrupts_.pop();

        if (active_)
        {
            if (maxLevel_ < 0 || getCurrentLevel() < maxLevel_)
            {
                if (interrupt) 
                {
                    indent();

                    if (parents_.size() > 0) {
                        cout << " `- ";
                    }

                    cout << parent << " ... ";
                    
                }

                if (success) {
                    cout << getConfirmation() << " ";
                } else {
                    cout << "FAILURE ";
                    exit(EXIT_FAILURE);
                }

                printTime(timer);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Logger::eol() 
{
    if (active_)
    {
        if (maxLevel_ < 0 || getCurrentLevel() <= maxLevel_) {
            eol_ = true;
            cout << endl;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
void Logger::printTime(Timer& timer) 
{
    const float dt = timer.get();

    cout << "[" << round(100 * dt) / 100 << " s]" << endl;
}


////////////////////////////////////////////////////////////////////////////////
void Logger::indent()
{
    for (int i = 0; i < (int)parents_.size() - 1; i++)
    {
        cout << '|';

        for (int j = 0; j < 1; ++j) {
            cout << ' ';
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
//  initialize logger
bool Logger::eol_ = true;
bool Logger::active_ = true;
int Logger::maxLevel_ = -1;
string Logger::confType_ = "doge";

stack<string> Logger::parents_;
stack<Timer> Logger::timers_;
stack<bool> Logger::interrupts_;



////////////////////////////////////////////////////////////////////////////////
const char* confs[] =
{
    "OK",
    "HERVORRAGEND",
    "AUSGEZEICHNET",
    "OKI DOKI",
    "GUDDI",
    "EXZELLENT",
    "TOP",
    "BRILLIANT",
    "BASST",
    "GUAD",
    "JAP",
    "YO",
    "ALLES KLAR",
    "SUBBA",
    "JAWOI",
    "MHM",
    "LAEUFT",
    "BAEM",
    "SAUBER",
    "RIGHT",
    "OIS KLAAA",
    "NEHMA MIT",
    "LASS MA DURCHGEHN",
    "KLAPPT",
    "GANZ OK",
    "SCHO RECHT",
    "FRALLE",
    "EH KLAR",
    "SOWIESO",
    "IMMER DOCH",
    "WUNDERBAR",
    "VERZUECKEND",
    "ENTZUECKEND",
    "FESCH",
};

const char* girlConfs[] = 
{
    "SUNSHINE",
    "SPARKLE",
    "PLEASES",
    "FABULOUS",
    "UNICORN",
    "RAINBOW",
};

const char* dogeConfs[] =
{
    "WOW",
    "SUCH SUCCESS",
    "MANY GOOD",
    "VERY OK",
    "MUCH GAIN",
    "SO ADVANCE",
    "SUCH WIN",
    "MANY TRIUMPH",
    "VERY ACHIEVE",
    "MUCH HAPPY",
    "SO VICTORY",
    "MANY ACCOMPLISHMENT",
};

const size_t confSize = sizeof(confs) / sizeof(confs[0]);
const size_t girlConfSize = sizeof(girlConfs) / sizeof(girlConfs[0]);
const size_t dogeConfSize = sizeof(dogeConfs) / sizeof(dogeConfs[0]);

vector<string> Logger::confirmations_(confs, confs + confSize);
vector<string> Logger::confirmationsGirl_(girlConfs, girlConfs + girlConfSize);
vector<string> Logger::confirmationsDoge_(dogeConfs, dogeConfs + dogeConfSize);