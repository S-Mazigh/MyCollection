#include <iostream>
#include <string>

namespace prettyPrint{
/***
 * Boxing a text with optional padding values
 * @param paddingHor gives the space to leave between the text and the left and right borders
 * * @param paddingVer gives the space to leave between the text and the upper and bottom borders
*/
void boxedPrint(const std::string &toprint, int paddingHor = 1, int paddingVer = 1)
{
    // +====================+
    // |                    |
    // |padding_text_padding|
    // |                    |
    // +====================+
    int printSize = toprint.size();
    std::cout << "+" << std::string(printSize + paddingHor*2, '=') << "+" << std::endl; // upper border
    // instanciate objects even if padding = 0, but it is unlikely to call this function with 0 padding
    // i think all the branching needed for economizing these instanciations makes the code ugly.

    std::string strPaddingVer(printSize + paddingHor*2, ' '); 
    std::string strPaddingHor(paddingHor, ' ');
    
    // adding the upper vertical padding
    for (int i = 0; i < paddingVer; i++)
        std::cout << "|" << strPaddingVer << "|" << std::endl;

    std::cout << "|" << strPaddingHor << toprint << strPaddingHor << "|" << std::endl;

    // adding the bottom vertical padding
    for (int i = 0; i < paddingVer; i++)
        std::cout << "|" << strPaddingVer << "|" << std::endl;

    std::cout << "+" << std::string(printSize + paddingHor*2, '=') << "+" << std::endl; // bottom border
}
}