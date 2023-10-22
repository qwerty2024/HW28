#include <iostream>
#include <future>
#include <chrono>

using namespace std;

bool make_thread = true; // флаг на использование многопоточности

void merge(int* arr, int l, int m, int r)
{
    int nl = m - l + 1;
    int nr = r - m;

    // создаем временные массивы
    //int left[nl], right[nr];
    int* left = new int[nl];
    int* right = new int[nr];

    // копируем данные во временные массивы
    for (int i = 0; i < nl; i++)
        left[i] = arr[l + i];
    for (int j = 0; j < nr; j++)
        right[j] = arr[m + 1 + j];

    int i = 0, j = 0;
    int k = l;  // начало левой части

    while (i < nl && j < nr) {
        // записываем минимальные элементы обратно во входной массив
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        }
        else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    // записываем оставшиеся элементы левой части
    while (i < nl) {
        arr[k] = left[i];
        i++;
        k++;
    }
    // записываем оставшиеся элементы правой части
    while (j < nr) {
        arr[k] = right[j];
        j++;
        k++;
    }

    delete[] left;
    delete[] right;
}

void mergeSort(int *arr, int l, int r)
{
    if (l >= r)
        return;

    int m = (l + r - 1) / 2;

    if (make_thread && (r - l > 100000))
    {
        // вызываем асинхронно рекурсию для правой части
        future<void> f = async(launch::async, [&](){ mergeSort(arr, l, m); } );
        mergeSort(arr, m + 1, r);
        f.get();
    }
    else {
        // запускаем обе части синхронно
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
    }

    merge(arr, l, m, r);
}

void printer(int *arr, long n) 
{
    for (long i = 0; i < n; ++i) 
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

bool test(int* arr, long n)
{
    for (long i = 1; i < n; ++i)
    {
        if (arr[i] < arr[i - 1]) return false;
    }

    return true;
}

int main()
{
    srand(0);
    long arr_size = 10000000;
    int* array = new int[arr_size];
    for (long i = 0; i < arr_size; i++) {
        array[i] = rand() % 500000;
    }
   

    auto t_start = chrono::system_clock::now();
    mergeSort(array, 0, arr_size - 1);
    auto t_end = chrono::system_clock::now();

    long diff = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
    std::cout << "Time(ms) : " << diff << std::endl;

    if (test(array, arr_size)) cout << "Test ok!" << endl; // проверка на верность результата
    else cout << "Test fail!" << endl;



    // обнуление для второго теста
    for (long i = 0; i < arr_size; i++) 
    {
        array[i] = rand() % 500000;
    }

    make_thread = false; // отключить потоки
    t_start = chrono::system_clock::now();
    mergeSort(array, 0, arr_size - 1);
    t_end = chrono::system_clock::now();

    diff = chrono::duration_cast<chrono::milliseconds>(t_end - t_start).count();
    std::cout << "Time(ms) : " << diff << std::endl;
    
    if (test(array, arr_size)) cout << "Test ok!" << endl; // проверка на верность результата
    else cout << "Test fail!" << endl;



    delete[] array;

    return 0;
}

