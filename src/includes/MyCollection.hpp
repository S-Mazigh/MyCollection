#pragma once
#include <vector>
#include <iostream>

namespace mycollections
{

    template <typename T>
    class MyCollection
    {
    private:
        bool sorted;
        bool ascending;
        std::vector<T> collection;

    public:
        MyCollection(std::vector<T> &collection); // checks if collection is sorted
        // sorting algos: ascending if true, descending if false
        void bubbleSort(bool ascending = true);
        void selectionSort(bool ascending = true);
        void insertionSort(bool ascending = true);
        void mergeSort(bool ascending = true);
        void quickSort(bool nascending = true);
        void heapSort(bool ascending = true);

        // searching algos (comming in V0.5)
        // int binarySearch(T value); // only if sorted
        // int normalSearch(T value);

        // getters
        bool isSorted() const { return this->sorted;}
        bool isAscending() const { return this->ascending; }
        // std::vector<T>& getCollection () const;

        // setters
        void setCollection (std::vector<T> &collection) { this->collection=collection; }

        // printing
        friend std::ostream &operator<<(std::ostream &stream, const MyCollection<T> &self)
        {
            int i;
            for (i = 0; i < self.collection.size() - 1; i++)
            {
                stream << self.collection[i] << ", ";
            }
            stream << self.collection[i] << ".";
            return stream;
        }
        ~MyCollection();
    };

    /**
     * Since this is a template class, i cannot do the definitions in a seperate cpp file.
     * Either define everything in the .hpp file of the class declaration, or make two seperate hpp files
     * Or manually instantiating the different possible implementations
     * https://www.reddit.com/r/cpp/comments/wcskvj/do_you_think_template_classes_will_ever_be_able/
     * https://stackoverflow.com/questions/10632251/undefined-reference-to-template-function
     * https://www.cs.technion.ac.il/users/yechiel/c++-faq/templates-defn-vs-decl.html
     * In short, the compiler need to see the use of the template class with a specific type and the code of the template at the same time (same file).
     */

    template <typename T>
    MyCollection<T>::MyCollection(std::vector<T> &collection) : collection(collection)
    {
        // need to check if T has operators < and > defined
        this->sorted = false;
        // check if already sorted ascending
        bool isAscending = true, isDescending = true;
        for (int i = 0; i < collection.size() - 1; i++)
        {
            if (collection[i] > collection[i + 1] && isAscending)
            {
                isAscending = false;
                return;
            }
            else if (collection[i] < collection[i + 1] && isDescending)
            {
                isDescending = false;
                return;
            }
            else if (!isDescending && !isAscending)
                break;
        }

        this->sorted = true;
        if (isAscending && !isDescending)
            this->ascending = true;
        else if (isDescending && !isAscending)
            this->ascending = false;
        else
            std::cerr << "Something wrong happened, a collection cannot be sorte ascending wise and descending wise at the same time!!!" << std::endl;
    }

    template <typename T>
    MyCollection<T>::~MyCollection()
    {
        /* does nothing */
        std::cout << "Bye cruel world!" << std::endl;
    }

    template<typename T> 
    inline void swap(T &val1, T &val2) {
       // std::cout << val1 <<" <-> "<< val2 << std::endl;
        T tmp = val1;
        val1 = val2;
        val2 = tmp;
    }


    /**
     * Simplest algorithm: Sort an element per loop by placing it at the end of the collection
    */
    template <typename T>
    void MyCollection<T>::bubbleSort(bool ascending)
    {
        if (this->sorted && !(ascending^this->ascending) ) // if already sorted in wanted order: !(a^b) means a==b. 
        {
            std::cout << "Collection is already sorted!"  << std::endl;
            return;
        }
        this->ascending = ascending;
        //int tmp;
        /*
         * the last i elements are always sorted
         */
        if (ascending)
        {
            for (int i = 0; i < this->collection.size(); i++)
            {
                this->sorted = true;
                for (int j = 0; j < this->collection.size() - i - 1; j++)
                {
                    if (this->collection.at(j) > this->collection.at(j+1))
                    {
                        swap(this->collection.at(j), this->collection.at(j+1));
                        this->sorted = false;
                    }
                }
            }
            return;
        }
        else
        {
            for (int i = 0; i < this->collection.size(); i++)
            {
                this->sorted = true;
                for (int j = 0; j < this->collection.size() - i - 1; j++)
                {
                    if (this->collection.at(j) < this->collection.at(j+1))
                    {
                        swap(this->collection.at(j), this->collection.at(j+1));
                        this->sorted = false;
                    }
                }
            }
        }
    }


    /*
    * At each round find max and min, and swaps them with the needed position
    * first swaps the min then the max
    * variable 's' stores the starting position in a round, while 'e' stores the end position.
    */
    template <typename T>
    void MyCollection<T>::selectionSort(bool ascending)
    {
        if (this->sorted && !(ascending^this->ascending) ) // if already sorted in wanted order: !(a^b) means a==b. 
        {
            std::cout << "Collection is already sorted!"  << std::endl;
            return;
        }

        this->ascending = ascending;
        int min, max; // stores the indexes of the max and min
        int s, e; // stores the start and end indexes of this round
        if (ascending)
        {
            
            for (s = 0; s < (this->collection.size()/2) ; s++) // if even then size/2 rounds, if odd size-1/2 rounds are enough (only one element in last round)
            {
                e = this->collection.size() - 1 - s;
                min = s;
                max = s;
                for (int j = s; j <= e; j++)
                {
                    if (this->collection.at(j) > this->collection.at(max)) // gets the max's index
                        max = j;
                    if (this->collection.at(j) < this->collection.at(min)) // gets the min's index
                        min = j;
                }
                // ascending so max to the end, and min to the start
                // std::cerr << "s = " << s << " ,e = "<<e<<" , max = "<< this->collection.at(max) << " ,min = " << this->collection.at(min) <<" collection = "<< *this << std::endl;
                swap(this->collection.at(max), this->collection.at(e)); // if min is at e, i will lose my min
                if(min == e)
                    min = s; // if min was at e, know it is at index max as a consquence of previous swap
                swap(this->collection.at(min), this->collection.at(s));
            }
            this->sorted = true;
            return;
        }
        else
        {
            for (s = 0; s < (this->collection.size()/2) ; s++) // if even then size/2 rounds, if odd size-1/2 rounds are enough (only one element in last round)
            {
                e = this->collection.size() - 1 - s;
                min = s;
                max = s;
                for (int j = s; j <= e; j++)
                {
                    if (this->collection.at(j) > this->collection.at(max)) // gets the max's index
                        max = j;
                    if (this->collection.at(j) < this->collection.at(min)) // gets the min's index
                        min = j;
                }
                 // descending so max at s and min at e
                // std::cerr << "s = " << s << " ,e = "<<e<<" , max = "<< this->collection.at(max) << "("<<max<<") ,min = " << this->collection.at(min) << "("<<min<<") collection = "<< *this << std::endl;
                swap(this->collection.at(max), this->collection.at(s)); // if min is at s: i will lose my min
                if(min == s)
                    min = max; // if min was at s know it is at max as a consequence of the previous swap
                swap(this->collection.at(min), this->collection.at(e));
            }
            this->sorted = true;
        }
    }

    /**
     * Ressemble un peu à la selection sort. Ici on essaie de trouver la place d'un element
     * au lieu de trouver le min ou le max pour le placer au debut ou la fin de la liste
     * Ici on essaie d'avancer tout en triant la table, du coup on aura une partie pseudo-trié
     * et une autre partie non triée.
     * 
     * Seul avantage: si liste déja triée complexité O(N), mais ce cas est en O(1) dans toutes 
     * les fonctions de cette classe gràce aux attribut sorted et ascending
     */
    template<typename T>
    void MyCollection<T>::insertionSort(bool ascending) {
        if (this->sorted && !(ascending^this->ascending) ) // if already sorted in wanted order: !(a^b) means a==b. 
        {
            std::cout << "Collection is already sorted!"  << std::endl;
            return;
        }
        this->ascending = ascending;

        signed int j;
        if (ascending)
        {
            for (int i = 0; i < this->collection.size()-1 ; i++) // to size - 1 because of j+1
            {
              j = i;
              
              // on essaie de trouver la bonne place pour l'element j
              // on check d'abord la valeur de j pour ne pas avoir d'erreur d'acces pour le .at(j)
              while (j >= 0 && this->collection.at(j) > this->collection.at(j+1)) {
                    swap(this->collection.at(j),this->collection.at(j+1)); // l'element à j n'est plus le même
                    j--;
                }
            }
            this->sorted = true;
            return;
        }else {
            for (int i = 0; i < this->collection.size()-1 ; i++) // to size - 1 because of j+1
            {
              j = i;
              
              // on essaie de trouver la bonne place pour l'element j
              while (j >= 0 && this->collection.at(j) < this->collection.at(j+1)) {
                    swap(this->collection.at(j),this->collection.at(j+1)); // l'element à j n'est plus le même
                    j--;
                }
            }
            this->sorted = true;
        }
    }
}
