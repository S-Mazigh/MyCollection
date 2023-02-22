#pragma once
#include <string>
#include <sstream>
#define DEBUG_LOG 0 // everything is printed
#define INFO_LOG 1  // only necessary information is printed
#define ERROR_LOG 2 // should always be printed

namespace myprint
{
    void boxedPrint(const std::string &toprint, int paddingHor = 1, int paddingVer = 1);

    class logger : public std::stringstream
    {
    private:
        char log_level;

    public:
        static char CURRENT_LEVEL;

        /**
         * Par défault le niveau du logger est set à debug_log pour ne pas afficher ses log quand le current_level est à info ou error
         */
        logger()
        {
            this->log_level = DEBUG_LOG;
        }

        logger(char level) : log_level(level)
        {
            std::cout << "Logger is initiated with level: " << log_level << std::endl;
        }

        /**
         * si le niveau du log est plus important que le niveau courant j'affiche
         * Par exemple si le niveau courant est à INFO = 1, j'affiche que le niveau INFO et ERROR vu que ERROR > INFO
         * Par défaut le niveau du log est à DEBUG et le niveau courant est à ERROR.
         */
        void log() const
        {
            if (this->log_level > CURRENT_LEVEL)
                std::cout << this->str();
        }

        /**
         * Si on veut utiliser un autre niveau que celui offert lors de l'instantiation sans faire un set
         * Elle offre plus de fléxbilité
         */
        void log(char level)
        {
            if (level > CURRENT_LEVEL)
                std::cout << this->str();
        }

        /**
         * Il est pas toujours nécessaire d'instantier une stringstream quand on print juste un string classique.
         */
        static void log(const std::string &to_print, char level = 0);

        void setLogLvl(char log_level)
        {
            this->log_level = log_level;
        }
    };

    /**
     * Les variables classique doivent être definie à l'exterieur de la class, parce que la classe ne définie pas c'est variable pour ne pas causer une erreur definition multiple
     * https://en.wikipedia.org/wiki/One_Definition_Rule#Definitions_of_static_const_data_members
     * */
}