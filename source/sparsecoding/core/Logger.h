#ifndef LOGGER_H
#define LOGGER_H


#include <iostream>
#include <vector>
#include <string>
#include <stack>

#include "stdafx.h"    //  new ostream types

using namespace std;


class Logger
{
    public:

        Logger(const int prio = 0);
        Logger(const string& msg, const int prio = 0);
        ~Logger();

        void push(const string& parent);
        void pop(bool success = true);
        void eol();

        template <class T>
        Logger& operator<< (const T& info)
        {
            if (active_)
            {
                if (maxLevel_ < 0 || getCurrentLevel() <= maxLevel_)
                {
                    if(!interrupts_.empty()) 
                    {
                        if (!interrupts_.top()) {
                            interrupts_.top() = true;
                            cout << endl;
                            eol_ = true;
                        }
                    }

                    if (eol_) {
                        indent();
                        cout << "|-- ";
                        eol_ = false;
                    }

                }

                cout << info;
            }

            return *this;
        };


        Logger& operator<< (std::ostream& (*f)(std::ostream&)) 
        {
            if (active_)
            {
                f(std::cout);

                if (maxLevel_ < 0 || getCurrentLevel() <= maxLevel_) {
                    eol_ = true;
                }
            }

            return *this;
        };


    private:

        int prio_;


    public: 

        static void setConfirmationType(const string& type) { confType_ = type; };
        static void setMaxLevel(const int level);

        static const string& getConfirmation();
        static const int getCurrentLevel();

        static void setActive(const bool active) { active_ = active; };


    private: 

        static bool eol_;
        static bool active_;
        static int maxLevel_;
        static string confType_;

        static stack<string> parents_;
        static stack<Timer> timers_;
        static stack<bool> interrupts_;

        static vector<string> confirmations_;
        static vector<string> confirmationsGirl_;
        static vector<string> confirmationsDoge_;

        static void indent();
        static void printTime(Timer& timer);

};


#endif