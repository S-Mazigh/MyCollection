## Description
- Une template class c++ enveloppante de `std::vector` qui offre quelques algorithmes de trie compilable via *cmake*:
  - **Linear with little memory overhead:**
    - *bubbleSort*: O(n²)
    - *insertionSort*: O(n²). Un tout petit peu plus rapide que bubbleSort. (En recherche d'optimisation)
    - *selectionSort*: O(n²). Trie le max et le min en une boucle. Plus rapide que bubbleSort et insertionSort.
  - **Divide and conquer algorithms:**
    - *mergeSort*: O(n*logn) en moyenne. Implementation avec récursion
    - *quickSort*: O(n*logn) en moyenne. Implementation sans récursion. Temps d'exécution plus long que le **mergeSort** (en moyenne 3x).
    - *heapSort*: ~O(n*logn) en moyenne. Implementation sans récursion. Temps d'exécution 5x plus important que le **mergeSort**.
- La librairie **MyPrintLibrary** est juste là pour avoir quelques fonctions de print plus interessants.
- Pour compiler le main:
```sh
cd path_to_repo_directory
mkdir build
cd build
cmake ../src
cmake --build
./Test 15000 # pour tester les algorithme avec un vecteur de taille 15000.
```
> NB: la collection de test est générée avec std::uniform_int_distribution<> et std::mt19937 comme generateur. La fonction génére un pseudo nombre aleatoire entre 0 et 2¹⁶.