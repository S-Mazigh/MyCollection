#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <cctype>
#include <cmath> // pour le log2 du quicksort (C11 nécessaire)
#include "PrettyPrinting.hpp"

namespace mycollections
{
    // for pseudo random number generator
    // here using creates aliases for the long named types
    using u32 = uint_least32_t; // u32 is the smallest datatype supported by the system that can hold at least 32 bits
    using engine = std::mt19937;

    template <typename T>
    class MyCollection
    {
    private:
        bool sorted;
        bool ascending;
        std::vector<T> collection;

        // For mergeSort
        void trueMergeSortRecursiveAscending(T *const collection, int const midpoint, int const end);
        void trueMergeSortRecursiveDescending(T *const collection, int const midpoint, int const end);

        // For heapSort
        void heapifyAscending(int size, int root);
        void heapifyDescending(int size, int root);

    public:
        MyCollection(std::vector<T> &collection); // checks if collection is sorted
        MyCollection(MyCollection &other);
        // sorting algos: ascending if true, descending if false
        void bubbleSort(bool ascending = true);
        void selectionSort(bool ascending = true);
        void insertionSort(bool ascending = true);

        // algorithms with divide and conquer approach
        void mergeSortRecursive(bool ascending = true);
        // void mergeSort(bool ascending = true);
        void quickSort(bool ascending = true, int maxLevels = 64); // par défaut support 2⁶⁴ niveaux
        void heapSort(bool ascending = true);

        // searching algos (comming in V0.5)
        int binarySearch(T value); // only if sorted
        int normalSearch(T value);

        // getters
        bool isSorted() const { return this->sorted; }
        bool isAscending() const { return this->ascending; }
        std::vector<T> getCollection() const { return this->collection; }

        // setters
        void setCollection(std::vector<T> &collection) { this->collection = collection; }

        // useful
        void shamble(); // to be called after sorting to resort the elements in a random way

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

    /**
     * Dans les fonctions qui suivent la condition du ascending enveloppe toute la boucle de trie pour avoir le minimum possible de branchements (de ifs)
     */
    template <typename T>
    MyCollection<T>::MyCollection(std::vector<T> &collection) : collection(collection)
    {
        // need to check if T has operators < and > defined
        this->sorted = false;
        // check if already sorted ascending or descending wise
        bool isAscending = true, isDescending = true;
        for (int i = 0; i < collection.size() - 1; i++)
        {
            if (collection[i] > collection[i + 1] && isAscending)
            {
                isAscending = false;
            }
            else if (collection[i] < collection[i + 1] && isDescending)
            {
                isDescending = false;
            }
            else if (!isDescending && !isAscending)
                return;
        }

        this->sorted = true;
        if (isAscending && !isDescending)
            this->ascending = true;
        else if (isDescending && !isAscending)
            this->ascending = false;
        else
            std::cerr << "Something wrong happened, a collection cannot be sorted ascending wise and descending wise at the same time!!!" << std::endl;
    }

    template <typename T>
    MyCollection<T>::MyCollection(MyCollection &other) : collection(other.collection), sorted(other.sorted), ascending(other.ascending)
    {
        // std::cout<<"Other collection is at " << &other.collection[0] << " while this' collection is at: " << &this->collection[0] << std::endl;
    }

    template <typename T>
    MyCollection<T>::~MyCollection()
    {
        /* does nothing */
        std::cout << "Bye cruel world!" << std::endl;
    }

    template <typename T>
    inline void swap(T &val1, T &val2)
    {
        // std::cout << val1 <<" <-> "<< val2 << std::endl;
        T tmp = val1;
        val1 = val2;
        val2 = tmp;
    }

    template <typename T>
    void print_array(T *array, int size)
    {
        std::cout << "\t";
        for (int i = 0; i < size; i++)
            std::cout << "[" << i << "]:" << array[i] << " ";
        std::cout << std::endl;
    }

    /**
     * To reordonne the collection in a random way.
     */
    template <typename T>
    void MyCollection<T>::shamble()
    {
        // std::cout << "Before: "<< *this << std::endl;
        //  setting the pseudo random number generator
        std::random_device os_seed; // in linux it uses /dev/random
        const u32 seed = os_seed();

        engine generator(seed);
        std::uniform_int_distribution<u32> distribute(0, this->collection.size() - 1);

        T tmp;
        u32 r;

        for (int i = 0; i < this->collection.size(); i++)
        {
            r = distribute(generator);
            tmp = this->collection.at(i);
            this->collection.at(i) = this->collection.at(r);
            this->collection.at(r) = tmp;
        }

        // std::cout << "After: "<< *this << std::endl;
    }

    /**
     * Simplest algorithm: Sort an element per loop by placing it at the end of the collection
     */
    template <typename T>
    void MyCollection<T>::bubbleSort(bool ascending)
    {
        if (this->sorted && !(ascending ^ this->ascending)) // if already sorted in wanted order: !(a^b) means a==b.
        {
            std::cout << "Collection is already sorted!" << std::endl;
            return;
        }
        this->ascending = ascending;
        // int tmp;
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
                    if (this->collection.at(j) > this->collection.at(j + 1))
                    {
                        swap(this->collection.at(j), this->collection.at(j + 1));
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
                    if (this->collection.at(j) < this->collection.at(j + 1))
                    {
                        swap(this->collection.at(j), this->collection.at(j + 1));
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
        if (this->sorted && !(ascending ^ this->ascending)) // if already sorted in wanted order: !(a^b) means a==b.
        {
            std::cout << "Collection is already sorted!" << std::endl;
            return;
        }

        this->ascending = ascending;
        int min, max; // stores the indexes of the max and min
        int s, e;     // stores the start and end indexes of this round
        if (ascending)
        {

            for (s = 0; s < (this->collection.size() / 2); s++) // if even then size/2 rounds, if odd size-1/2 rounds are enough (only one element in last round)
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
                if (min == e)
                    min = s; // if min was at e, know it is at index max as a consquence of previous swap
                swap(this->collection.at(min), this->collection.at(s));
            }
            this->sorted = true;
            return;
        }
        else
        {
            for (s = 0; s < (this->collection.size() / 2); s++) // if even then size/2 rounds, if odd size-1/2 rounds are enough (only one element in last round)
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
                if (min == s)
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
     * les fonctions de cette classe gràce aux attributs sorted et ascending
     */
    template <typename T>
    void MyCollection<T>::insertionSort(bool ascending)
    {
        if (this->sorted && !(ascending ^ this->ascending)) // if already sorted in wanted order: !(a^b) means a==b.
        {
            std::cout << "Collection is already sorted!" << std::endl;
            return;
        }
        this->ascending = ascending;

        signed int j;
        if (ascending)
        {
            for (int i = 0; i < this->collection.size() - 1; i++) // to size - 1 because of j+1
            {
                j = i;

                // on essaie de trouver la bonne place pour l'element j dans la partie triée
                // on check d'abord la valeur de j pour ne pas avoir d'erreur d'acces pour le .at(j)
                while (j >= 0 && this->collection.at(j) > this->collection.at(j + 1))
                {
                    swap(this->collection.at(j), this->collection.at(j + 1)); // l'element à j n'est plus le même
                    j--;
                }
            }
            this->sorted = true;
            return;
        }
        else
        {
            for (int i = 0; i < this->collection.size() - 1; i++) // to size - 1 because of j+1
            {
                j = i;

                // on essaie de trouver la bonne place pour l'element j
                while (j >= 0 && this->collection.at(j) < this->collection.at(j + 1))
                {
                    swap(this->collection.at(j), this->collection.at(j + 1)); // l'element à j n'est plus le même
                    j--;
                }
            }
            this->sorted = true;
        }
    }

    /**
     * 1- Split en deux sub arrays
     * 2- Une fois sub array d'un element ou zero arret
     * 3- Merge et sort les elements
     * Dans le merge il faut faire attention à bien verifier le cas des égalité vu que là en copie des valeurs et non les swapper (autrement dit on peut les perdre)
     */
    template <typename T>
    void MyCollection<T>::mergeSortRecursive(bool ascending)
    {
        if (this->sorted && !(ascending ^ this->ascending)) // if already sorted in wanted order: !(a^b) means a==b.
        {
            std::cout << "Collection is already sorted!" << std::endl;
            return;
        }

        // r est le end enfaite
        int r = this->collection.size() - 1;
        if (r <= 0) // un seul element ou vide
            return;
        int m = r / 2;

        // std::cout<<"Calling true recursive"<<std::endl;
        if (ascending)
            trueMergeSortRecursiveAscending(&this->collection[0], m, r);
        else
            trueMergeSortRecursiveDescending(&this->collection[0], m, r);
        this->sorted = true;
        this->ascending = ascending;
    }

    template <typename T>
    void MyCollection<T>::trueMergeSortRecursiveAscending(T *const collection, int const midpoint, int const end)
    {
        // std::cout << "Initiating method with midpoint: "<<midpoint<<", end: "<<end<<std::endl;
        //  recursion base
        if (end <= 0)
        { // if one element or empty
            // std::cout<<" Base condition reached "<< std::endl;
            return;
        }

        // Enough elements are present
        // creating the temp arrays
        int leftSize = midpoint + 1;
        int rightSize = end - midpoint;

        T *const leftArray = new T[leftSize]; // const after * then pointer is const
        T *const rightArray = new T[rightSize];

        // [0] to [midpoint]
        for (int i = 0; i < leftSize; i++)
        {
            leftArray[i] = collection[i];
        }

        // [midpoint + 1] to [end]
        for (int i = 0; i < rightSize; i++)
        {
            rightArray[i] = collection[i + midpoint + 1];
        }

        // std::cout << " Recursion: left:"<< leftSize << ", right:" << rightSize << std::endl;
        //  recursion: going down
        trueMergeSortRecursiveAscending(leftArray, (leftSize - 1) / 2, leftSize - 1);
        trueMergeSortRecursiveAscending(rightArray, (rightSize - 1) / 2, rightSize - 1);

        // recursion: going up
        // we need to merge and sort
        int leftIndex = 0;
        int rightIndex = 0;
        int mergedIndex = 0;
        int mergedSize = end + 1; // variable can be removed

        /**
         * Les indexes des subArrays s'increment de facon independante
         * Si l'element d'un subarray est ajouté au mergedArray (collection) son index s'incremente
         * On ajoute à chaque fois l'element le plus petit (grand) entre les premiers elements de chaque subarray
         * Il suffit de comparer les premiers elements de chaque subarray parce qu'on est sur qu'ils sont triés
         */

        // std::cout << "Merging" << std::endl;
        // std::cout << " Left Array: "<<std::endl;
        // print_array(leftArray, leftSize);
        // std::cout << " Right Array: "<<std::endl;
        // print_array(rightArray, rightSize);

        for (int i = 0; i < mergedSize; i++)
        { // ascending order
            if (leftIndex >= leftSize || rightIndex >= rightSize)
            {
                mergedIndex = i;
                break; // break if one subarray has no more elements
            }
            if (leftArray[leftIndex] <= rightArray[rightIndex])
            {
                collection[i] = leftArray[leftIndex];
                leftIndex++;
            }
            else // if (rightArray[rightIndex] < leftArray[leftIndex])
            {
                collection[i] = rightArray[rightIndex];
                rightIndex++;
            }
        }

        // std::cout << " Adding remaining elements: left:"<< leftSize << "(" << leftIndex << "), right:" << rightSize << "(" << rightIndex << ")" << std::endl;
        //  Ajout des elements restants du subarray restant
        if (leftIndex >= leftSize) // du coup il en reste à droite
        {
            for (int i = mergedIndex; i < mergedSize; i++)
            {
                collection[i] = rightArray[rightIndex];
                rightIndex++;
            }
        }
        else if (rightIndex >= rightSize)
        { // il en reste à gauche
            for (int i = mergedIndex; i < mergedSize; i++)
            {
                collection[i] = leftArray[leftIndex];
                leftIndex++;
            }
        }

        // freeing ununsed memory
        delete[] rightArray;
        delete[] leftArray;
    }

    template <typename T>
    void MyCollection<T>::trueMergeSortRecursiveDescending(T *const collection, int const midpoint, int const end)
    {
        // std::cout << "Initiating method with midpoint: "<<midpoint<<", end: "<<end<<std::endl;
        //  recursion base
        if (end <= 0)
        { // if one element or empty
            // std::cout<<" Base condition reached "<< std::endl;
            return;
        }

        // Enough elements are present
        // creating the temp arrays
        int leftSize = midpoint + 1;
        int rightSize = end - midpoint;

        T *const leftArray = new T[leftSize]; // const after * then pointer is const
        T *const rightArray = new T[rightSize];

        // [0] to [midpoint]
        for (int i = 0; i < leftSize; i++)
        {
            leftArray[i] = collection[i];
        }

        // [midpoint + 1] to [end]
        for (int i = 0; i < rightSize; i++)
        {
            rightArray[i] = collection[i + midpoint + 1];
        }

        // std::cout << " Recursion: left:"<< leftSize << ", right:" << rightSize << std::endl;
        //  recursion: going down
        trueMergeSortRecursiveDescending(leftArray, (leftSize - 1) / 2, leftSize - 1);
        trueMergeSortRecursiveDescending(rightArray, (rightSize - 1) / 2, rightSize - 1);

        // recursion: going up
        // we need to merge and sort
        int leftIndex = 0;
        int rightIndex = 0;
        int mergedIndex = 0;
        int mergedSize = end + 1; // variable can be removed

        // std::cout << " Merging\n Left Array: "<<std::endl;
        // print_array(leftArray, leftSize);
        // std::cout << " Right Array: "<<std::endl;
        // print_array(rightArray, rightSize);
        for (int i = 0; i < mergedSize; i++)
        { // descending order
            if (leftIndex >= leftSize || rightIndex >= rightSize)
            {
                mergedIndex = i;
                break; // break if one subarray has no more elements
            }
            if (leftArray[leftIndex] >= rightArray[rightIndex])
            {
                collection[i] = leftArray[leftIndex];
                leftIndex++;
            }
            else // if (rightArray[rightIndex] > leftArray[leftIndex])
            {
                collection[i] = rightArray[rightIndex];
                rightIndex++;
            }
        }
        // std::cout << " Result One is " << std::endl;
        // print_array(collection, mergedSize);

        // std::cout << " Adding remaining elements: left:"<< leftSize << "(" << leftIndex << "), right:" << rightSize << "(" << rightIndex << ")" << std::endl;
        //  Ajout des elements restants du subarray restant
        if (leftIndex >= leftSize) // du coup il en reste à droite
        {
            for (int i = mergedIndex; i < mergedSize; i++)
            {
                collection[i] = rightArray[rightIndex];
                rightIndex++;
            }
        }
        else if (rightIndex >= rightSize)
        { // il en reste à gauche
            for (int i = mergedIndex; i < mergedSize; i++)
            {
                collection[i] = leftArray[leftIndex];
                leftIndex++;
            }
        }
        // std::cout << " Result Two is " << std::endl;
        // print_array(collection, mergedSize);
        // std::cout << "--------------------\n";
        //  freeing ununsed memory
        delete[] rightArray;
        delete[] leftArray;
    }

    /**
     * Pour l'instant le pivot est toujours le dernier elément, une future version donnera le choix à l'utilisateur de le modifier.
     * Dans un round: l'index 's' garde l'index du dernier element du subarray de gauche.
     * Si un element est plus petit (grand) que le pivot ce dernier est swappé avec 's+1'
     * Les rounds se succede jusqu'à ce qu'il y est qu'un seul element dans le subarray restant
     * O(n²) si on trie un tableau pré ordonnée dans l'ordre inverse
     *
     * @param ascending est true par défaut
     * @param maxLevels est 64 par défaut, i.e. la fonction supporte des taille de tableau allant jusqu'à 2⁶⁴.
     */
    template <typename T>
    void MyCollection<T>::quickSort(bool ascending, int maxLevels)
    {

        // Pour eviter un fail relié à la taille limité des tableaux de mémoire des start et end j'ai ajouté
        if (log2(this->collection.size()) > (double)maxLevels)
        {
            myprint::logger::log("Attention! La taille de votre collection pourrait causer une erreur, ceci est due à la taille limitée des tableaux stockant les index de debut et de fin de chaque subarrays (starts[maxLevels] et ends[maxLevels]).\n Veuillez mettre comme deuxième parametre une valeur supérieure au log_2(taille de votre collection).\n", ERROR_LOG);
            return;
        }
        /**
         *  A chaque round on consomme un start et end, et on génére au max deux starts et deux ends.
         *  Les tableaux suivent une logique LIFO : ainsi on a besoin que d'une variable supplementaire le STACK POINTER.
         */
        int sp = 0;
        int start[maxLevels], end[maxLevels];

        // utilisées par chaque round de façon independante
        // int pivot;
        int s, start_round, end_round;

        // INITIALISATION
        start[sp] = 0;
        end[sp] = this->collection.size() - 1;
        sp++; // sp est à 1 avant d'entrer dans la boucle.

        if (ascending)
        {
            while (sp > 0) // s'arrete s'il reste plus de subarray à traverser
            {
                start_round = start[--sp]; // d'abord decremente puis récupére la valeur
                end_round = end[sp];
                // le pivot est toujours le dernier element
                // pivot = end_round;
                s = start_round - 1; // sans le -1 c'est comme si on supposait que le premier element est plus petit(grand) que le pivot
                for (int i = start_round; i < end_round; i++)
                {                                                                // des swaps inutiles peuvent arrivés, on s'arrete juste avant le pivot
                    if (this->collection.at(i) < this->collection.at(end_round)) // asceding order
                    {
                        s++;
                        swap(this->collection.at(s), this->collection.at(i));
                    }
                }
                // maintenant on met le pivot entre les deux subarray
                swap(this->collection.at(s + 1), this->collection.at(end_round));
                /**
                 * ajouter les deux subarrays generés
                 * Exemple: sp = 1 à l'entré de la boucle puis devient 0 aprés le start_round = start[--sp]
                 * Si les deux sont ajouté:
                 *  1 - start[0] reste comme il était, end[0] est ecrasé avec s et sp = 1
                 *  2 - start[1] est mis à jour, end[1] = end_round et sp = 2
                 * Si seulement le 1er est ajouté:
                 *  1 - start[0] reste comme il était, end[0] est ecrasé avec s et sp = 1
                 * Si seuelemnt le 2eme est ajouté:
                 *  2 - start[0] est mis à jour, end[0] = end_round et sp = 1. C'est vrai que le end[0] = end_round est inutile
                 * */
                if (start_round < s) // subarray de gauche a plus d'un element
                {
                    // start[sp] ne change pas
                    end[sp++] = s; // met la valeur puis incremente
                }
                if ((s + 2) < end_round) // subarray de droite a plus d'un element
                {
                    start[sp] = s + 2;
                    end[sp++] = end_round;
                }
            }
        }
        else
        {
            while (sp > 0) // s'arrete s'il reste plus de subarray à traverser
            {
                start_round = start[--sp];
                end_round = end[sp];
                // le pivot est toujours le dernier element
                // pivot = end_round;
                s = start_round - 1; // sans le -1 soit on suppose que le premier element est plus petit(grand) que le pivot
                for (int i = start_round; i < end_round; i++)
                {                                                                // des swaps inutiles peuvent arrivés, on s'arrete juste avant le pivot
                    if (this->collection.at(i) > this->collection.at(end_round)) // desceding order
                    {
                        s++;
                        swap(this->collection.at(s), this->collection.at(i));
                    }
                }
                // maintenant on met le pivot entre les deux subarray
                swap(this->collection.at(s + 1), this->collection.at(end_round));
                if (start_round < s) // subarray de gauche a plus d'un element
                {
                    // start[sp] ne change pas
                    end[sp++] = s;
                }
                if ((s + 2) < end_round) // subarray de droite a plus d'un element
                {
                    start[sp] = s + 2;
                    end[sp++] = end_round;
                }
            }
        }
    }

    /**
     * Heap Sort permet d'avoir une complexité proche du quicksort et mergesort sans utiliser de mémoire en plus.
     * Comme les subarrays dans merge sort et le stockage des index dans quicksort.
     *
     *  La collection va être lu comme un arbre binaire ou un noeud 'i' a ses enfants de droite et de gauche à 'i*2+1' et 'i*2+2' respectivement
     * Avec cette logique la deuxieme moitié de la collection ne comprendra que des feuilles:
     *  - Une collection de 11 elements [0..10] aura 6 feuilles: [0..4] sont des noeuds parents et [5..10] seront des feuilles

     * Le heap sort fait en sorte de swaper un parent avec son plus grand (petit) descendants en commencant par le bas (index 4 dans l'exemple).
     * En suivant cette méthode on aura à la fin d'une iteration le plus grand (petit) element comme racine. Ce dernier et deplacé vers sa place finale
     * dans la collection. Et en refait la même chose sans le max (min) trouvé dans les iterations precendents jusqu'à avoir une collection triée.
     */

    /**
     * Heapify va se charger de faire remonter le max vers la racine.
     * @param size pour ignorer les derniers elements (déja triées)
     * @param root nous permet de selectionner qu'une partie de l'arbre vu qu'on doit commencer du bas vers le haut pour trouver le max
     * A chaque iteration on doit verifier que l'enfant qui recoit la valeur du parent respecte la condition (contient la valeur la plus grande).
     *  - Par exemple quand le root contient le min cela arrivera souvent.
     */
    template <typename T>
    void MyCollection<T>::heapifyAscending(int size, int root)
    {
        int left, right;
        int largest = root;
        bool rootSwaped;
        do
        {
            rootSwaped = false;
            left = root * 2 + 1;
            right = root * 2 + 2; // left + 1;
            //std::cout << "Root: " << root << "(" << this->collection.at(root) << "), Left: " << left;
            if (left < size && collection.at(left) > collection.at(largest)) // largest = root à la premiere condition
            {
                //std::cout << "(" << this->collection.at(left) << ")";
                largest = left;
            }
            //std::cout << ", Right: " << right;
            if (right < size && collection.at(right) > collection.at(largest))
            {
                //std::cout << "(" << this->collection.at(right) << ")";
                largest = right;
            }
            //std::cout << std::endl;
            if (largest != root)
            {
                swap(collection.at(root), collection.at(largest));
                rootSwaped = true; // un enfant a changer du coup on doit revérifier la condition chez les enfants.
                root = largest;    // changement de root pour la prochaine iteration
            }

        } while (rootSwaped);
    }

    /**
     * Heapify va se charger de faire remonter le min vers la racine.
     * @param size pour ignorer les derniers elements (déja triées)
     * @param root nous permet de selectionner qu'une partie de l'arbre vu qu'on doit commencer du bas vers le haut pour trouver le min
     * A chaque iteration on doit verifier que l'enfant qui recoit la valeur du parent respecte la condition (contient la valeur la plus petite).
     *  - Par exemple quand le root contient le max cela arrivera souvent.
     */
    template <typename T>
    void MyCollection<T>::heapifyDescending(int size, int root)
    {
        int left, right;
        int smallest = root;
        bool rootSwaped;
        do
        {
            rootSwaped = false;
            left = root * 2 + 1;
            right = root * 2 + 2; // left + 1;
            //std::cout << "Root: " << root << "(" << this->collection.at(root) << "), Left: " << left;
            if (left < size && collection.at(left) < collection.at(smallest)) // smallest = root à la premiere condition
            {
                //std::cout << "(" << this->collection.at(left) << ")";
                smallest = left;
            }
            //std::cout << ", Right: " << right;
            if (right < size && collection.at(right) < collection.at(smallest))
            {
                //std::cout << "(" << this->collection.at(right) << ")";
                smallest = right;
            }
            //std::cout << std::endl;
            if (smallest != root)
            {
                swap(collection.at(root), collection.at(smallest));
                rootSwaped = true; // un enfant a changer du coup on doit revérifier la condition chez les enfants.
                root = smallest;   // changement de root pour la prochaine iteration
            }

        } while (rootSwaped);
    }

    template <typename T>
    void MyCollection<T>::heapSort(bool ascending)
    {
        if (this->sorted && !(ascending ^ this->ascending)) // if already sorted in wanted order: !(a^b) means a==b.
        {
            std::cout << "Collection is already sorted!" << std::endl;
            return;
        }

        if (ascending)
        {
            // On commence par faire un premier heapify pour avoir un arbre où chaque parent est plus grand ou égale à ses descendants.
            for (int i = (this->collection.size() / 2) - 1; i >= 0; i--)
            {
                heapifyAscending(this->collection.size(), i);
            }
            //std::cout << "First Heapify done: " << *this << std::endl;
            /*
            - Maintenant on va utiliser l'arbre obtenu pour trier notre collection dans l'ordre voulue.
            - Le premier heapify nous assure qu'un noeud est plus grand que ses descendants, i.e. il a comme enfants les plus grandes valeurs de ses sous-arbres
            - On faisant ca on forme un nouvelle arbre ou le root est le dernier elements de la collection (une feuille).
                - Du coup on doit remonter son parent et le comparer avec l'autre sous-arbre.
            */
            for (int i = this->collection.size() - 1; i >= 0; i--)
            {
                swap(this->collection.at(i), this->collection.at(0));
                /* Dans ce heapify on ne deplace que la valeur du nouveau root à son ancienne place (dernier element).
                On a pas besoin de le refaire pour tous les noeuds avec enfants, un noeud va perdre un enfants à chaque iteration c'est tout.
                */
                heapifyAscending(i, 0);
                //std::cout << "Heapify (" << i << ") done: " << *this << std::endl;
            }
        }
        else // descending
        {
            // On commence par faire un premier heapify pour avoir un arbre où chaque parent est plus petit ou égale à ses descendants.
            for (int i = (this->collection.size() / 2) - 1; i >= 0; i--)
            {
                heapifyDescending(this->collection.size(), i);
            }
            /*
            - Maintenant on va utiliser l'arbre obtenu pour trier notre collection dans l'ordre voulue.
            - Le premier heapify nous assure qu'un noeud est plus petit que ses descendants, i.e. il a comme enfants les plus petites valeurs de ses sous-arbres.
            - On faisant ca on forme un nouvelle arbre ou le root est le dernier elements de la collection (une feuille).
                - Du coup on doit remonter son parent et le comparer avec l'autre sous-arbre.
            */
            for (int i = this->collection.size() - 1; i >= 0; i--)
            {
                swap(this->collection.at(i), this->collection.at(0));
                /* Dans ce heapify on ne deplace que la valeur du nouveau root à son ancienne place (dernier element).
                On a pas besoin de le refaire pour tous les noeuds avec enfants, un noeud va perdre un enfants à chaque iteration c'est tout.
                */
                heapifyDescending(i, 0);
                //std::cout << "Heapify (" << i << ") done: " << *this << std::endl;
            }
        }

        this->sorted = true;
        this->ascending = ascending;
    }
}
