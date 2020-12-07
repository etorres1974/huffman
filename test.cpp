#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>  
#include <iterator>
#include <sstream>
#include <iostream>
#include <charconv> 
using namespace std;

#define MAX_TREE_HT 50

class HuffmanCode
{
  public :
    char ch;
    vector<char> code;
};

struct MinHNode
{
  unsigned freq;
  char item;
  struct MinHNode *left, *right;
};

struct MinH
{
  unsigned size;
  unsigned capacity;
  struct MinHNode **array;
};

struct MinHNode *newNode(char item, unsigned freq)
{
  struct MinHNode *temp = (struct MinHNode *)malloc(sizeof(struct MinHNode));

  temp->left = temp->right = NULL;
  temp->item = item;
  temp->freq = freq;

  return temp;
}

struct MinH *createMinH(unsigned capacity)
{
  struct MinH *minHeap = (struct MinH *)malloc(sizeof(struct MinH));
  minHeap->size = 0;
  minHeap->capacity = capacity;
  minHeap->array = (struct MinHNode **)malloc(minHeap->capacity * sizeof(struct MinHNode *));
  return minHeap;
}

void swapMinHNode(struct MinHNode **a, struct MinHNode **b)
{
  struct MinHNode *t = *a;
  *a = *b;
  *b = t;
}

void minHeapify(struct MinH *minHeap, int idx)
{
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
    smallest = left;

  if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
    smallest = right;

  if (smallest != idx)
  {
    swapMinHNode(&minHeap->array[smallest],
                 &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

int checkSizeOne(struct MinH *minHeap)
{
  return (minHeap->size == 1);
}

struct MinHNode *extractMin(struct MinH *minHeap)
{
  struct MinHNode *temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];

  --minHeap->size;
  minHeapify(minHeap, 0);

  return temp;
}

void insertMinHeap(struct MinH *minHeap, struct MinHNode *minHeapNode)
{
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
  {
    minHeap->array[i] = minHeap->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }

  minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinH *minHeap)
{
  int n = minHeap->size - 1;
  int i;

  for (i = (n - 1) / 2; i >= 0; --i)
    minHeapify(minHeap, i);
}

int isLeaf(struct MinHNode *root)
{
  return !(root->left) && !(root->right);
}

struct MinH *createAndBuildMinHeap(char item[], int freq[], int size)
{
  struct MinH *minHeap = createMinH(size);

  for (int i = 0; i < size; ++i)
    minHeap->array[i] = newNode(item[i], freq[i]);

  minHeap->size = size;
  buildMinHeap(minHeap);

  return minHeap;
}

struct MinHNode *buildHfTree(char item[], int freq[], int size)
{
  struct MinHNode *left, *right, *top;
  struct MinH *minHeap = createAndBuildMinHeap(item, freq, size);

  while (!checkSizeOne(minHeap))
  {
    left = extractMin(minHeap);
    right = extractMin(minHeap);

    top = newNode('$', left->freq + right->freq);

    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top);
  }
  return extractMin(minHeap);
}

// Le o arquivo input.txt e retorna seu conteúdo como um vetor de char
vector<char> readCharFile(string path) {
    vector<char> v;
    ifstream in(path);
    char c;
    if(in.is_open()) {
        while(in.good()) {
            while(in.get(c)){
              v.push_back(c);
            }
        }
    }
    if(!in.eof() && in.fail())
        cout << "error reading " << path << endl;

    in.close();
    return v;
}

void printCharVector(vector<char> v){
  for(int i = 0; i < v.size(); i++){
    printf("\n Char[%i] : %c", i, v[i]);
  }
}

// Imprime array de int
void printArray(int arr[], int n)
{
  int i;
  for (i = 0; i < n; ++i)
    cout << arr[i];

  cout << "\n";
}


// Transforma a entrada em Huffmancode e salva em output.freq
void compress(vector<char> raw, vector<HuffmanCode> dictionary){ 
  ofstream output;
  output.open("output.freq");
  for(int r = 0; r < raw.size(); r++){
    for(int d = 0; d < dictionary.size(); d++){
      if(raw[r] == dictionary[d].ch){
        for(int i = 0; i < dictionary[d].code.size(); i++){
           output << dictionary[d].code[i];
        }
      }
    }
  }
  output.close();
}
//TODO descompressão here
void decompress(){ 
  ofstream output;
  output.open("output.defreq");
  output << " descompactado "; //Resultado da descompactação
  output.close();
}

void setCodeFromTree(int arr[], int n, char code[50])
{
  for (int i = 0; i < n; ++i){
    code[i] = arr[i];
  }
}
// Imprime recursivamente os nós
void printHCodes(struct MinHNode *root, int arr[], int top, vector<HuffmanCode> &hc)
{
  if (root->left)
  {
    arr[top] = 0;
    printHCodes(root->left, arr, top + 1, hc);
  }

  if (root->right)
  {
    arr[top] = 1;
    printHCodes(root->right, arr, top + 1, hc);
  }
  if (isLeaf(root))
  {
    cout << root->item << " : ";
    printArray(arr, top);
    vector<char> code;
    for (int i = 0; i < top; ++i){
     stringstream ss; 
     char c;
     ss << arr[i];
     ss >> c ;
     code.push_back(c);
    }
    // Salva o par no vetor recebido pra poder usar de voltan na main;
    for(int h = 0; h < hc.size(); h++){
        if( root->item ==  hc[h].ch){
          hc[h].code = code;
        }
    }
    
  }
}

// Recebe um vetor de char e retorna um novo vetor apenas com os elementos únicos.
vector<char> vectorToUnique(vector<char> z){
  vector<char> v;
  vector<char>::iterator it;

  copy(z.begin(), z.end(), back_inserter(v));
  sort(v.begin(), v.end());
  it = unique(v.begin(), v.end());
  v.resize(distance(v.begin(),it)); 
  return v ;
}

vector<int> getFrequencyVector(vector<char> raw, vector<char> unique){
  vector<int> freq;
  
  for(int u = 0 ; u < unique.size(); u++){
    int counter = 0;
    for(int r = 0; r < raw.size(); r++){
      if(unique[u] == raw[r]){
        counter ++;
      }
    }
    freq.push_back(counter);
  }
  return freq;
}


int main(){
  // Ler o arquivo de entrada input.txt e armazenar os dados em um vector
  vector<char> raw = readCharFile("input.txt");
  //printf("\n------Raw----");
  //printCharVector(raw);

  // Retira as repetições do vector para mapear todos os caractéres usados
  vector<char> unique = vectorToUnique(raw);
  //printf("\n------Unique ----");
  //printCharVector(unique);

  // Frequencia de cada elemento.
  vector<int> freq =  getFrequencyVector(raw, unique);
  //printf("\n------frequency ----");
  //for( int i = 0; i < freq.size(); i++){
  //  cout << "\nf " << i << " : " << freq[i];
  //}


  // Escrever a versão descompactada
  decompress();

  int size = unique.size() / sizeof(unique[0]);

  struct MinHNode *root = buildHfTree(&unique[0], &freq[0], size);
  // Um dicionário com a relação entre letra e código
  vector<HuffmanCode> hcVec;

  for(int h = 0; h < unique.size(); h++){
    HuffmanCode hc;
    hc.ch = unique[h];
    hcVec.push_back(hc);
  }
  int arr[MAX_TREE_HT], top = 0;

  cout << "Char : Code \n";
  printHCodes(root, arr, top, hcVec);

  compress(raw, hcVec);
  //Imprime tamanho dos arquivos
  /*
  Para isso considere como se os códigos de Huffman fossem compostos por
  dígitos binários, ou seja, cada símbolo do código ocupa apenas um bit, enquanto cada
  símbolo da entrada é um caractere (que ocupa 8 bits).
  */
  int inputSize = raw.size() * 8;
  vector<char> output = readCharFile("output.freq");
  vector<char> decompressed = readCharFile("output.defreq");
  float compressRatio = ( 1 - ((float) output.size() / inputSize) ) * 100;
  printf("\n Tamanho Entrada = %i bits", inputSize);
  printf("\n Tamanho Saida comprimida = %i bits", output.size());
  printf("\n Tamanho Saida descomprimida = %i bits", decompressed.size());
  printf("\n Taxa de compressão = %f %", compressRatio);
}

