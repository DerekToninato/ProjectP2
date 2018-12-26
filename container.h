#ifndef CONTAINER_H
#define CONTAINER_H

#include<iostream>
using std::ostream;
using std::endl;

template<class T> class Container;
template<class T> class Node;
template<class T> ostream& operator<< (ostream&, const Container<T>&);
template<class T> ostream& operator<< (ostream&, const Node<T>&);

template <class T>
class Container {
    friend class Iterator;

private:
    class Node {
        public:
            T info;
            Node*next;
            Node(const T& x): info(x), next(0) {}
            ~Node() {}
            friend ostream& operator<< <T> (ostream&, const Container<T>&);
            friend ostream& operator<< (ostream&, const Node&);
    };

    Node*first;
    static Node* copy(Node*);
    static void destroy(Node*);
    static bool LexicographicCompare(Node*, Node*);

//INTERFACCIA PUBBLICA   ---- TODO Iterator va fornito sia normale che costante ----
public:
    class Iterator {
        friend class Container<T>;

    private:
        Container::Node*point;

    public:
        bool operator== (Iterator) const;
        bool operator!= (Iterator) const;
        Iterator& operator++ ();
        Iterator operator++ (int);
        Node* operator-> () const;
        Node& operator* () const;
    };

    Container() : first(0) {}
    //triade profonda
    ~Container();
    Container(const Container&);
    Container& operator= (const Container&);
    friend ostream& operator<< <T> (ostream&, const Container<T>&);
    bool operator< (const Container&) const; //la comparazione lessicografica prevede molti più operatori (==, !=, ...)

    //metodi di Container forniti all'utente
    void Insert(const T);                   //aggiunge un nodo al contenitore
    void Remove(const T&);                  //rimuove un nodo al contenitore
    bool Search(const T&) const;            //trova l'occorrenza di un nodo all'interno del contenitore
    unsigned long Size() const;             //conta quanti nodi sono presenti all'interno del contenitore
    bool Empty() const;                     //controlla se il contenitore è vuoto
    void Clear();                           //svuota il contenitore

    //metodi forniti che usano iteratore
    Iterator Begin() const;
    Iterator End() const;  // ---- TODO controllare past di end con esercizio dList
};

//metodo privato copy
template <class T>
typename Container<T>::Node* Container<T>::copy(Node* p) {
    if(!p)
        return 0;
    else
        return new Node(p->info, copy(p->next));
}

//metodo privato destroy (dopo la destroy puntatore passato = nullptr, altrimenti dangling)
template<class T>
void Container<T>::destroy(Node* p) {
    Node*q;
    while(p)
    {
        q = p;
        p = p->next;
        delete q;
    }
}

//metodo privato lexigographicCompare
template <class T>
bool Container<T>::LexicographicCompare(Node* n1, Node* n2) {
    if(!n2) return false;
    if(!n1) return true;
    return n1->info < n2->info || (n1->info == n2->info && LexicographicCompare(n1->next,n2->next));
}

//distruttore profondo
template<class T>
Container<T>::~Container<T>() { destroy(first); first = nullptr; }

//cdc profondo
template<class T>
Container<T>::Container(const Container& c) : first(copy(c.first)) {}

//assegnazione profonda
template<class T>
Container<T>& Container<T>::operator= (const Container& c) {
    if(this != &c)
    {
        destroy(first);
        first = copy(c.first);
    }
    return *this;
}

//overload dell'operatore <
template<class T>
bool Container<T>::operator< (const Container& c) const { return LexicographicCompare(first, c.first); }

//operatori di Iterator
template<class T>
bool Container<T>::Iterator::operator== (Iterator i) const { return point == i.point; }

template<class T>
bool Container<T>::Iterator::operator!= (Iterator i) const { return point != i.point; }

template<class T> //attenzione se point == 0 l'operatore non fa nulla
typename Container<T>::Iterator& Container<T>::Iterator::operator++ () {
    if(point)
        point = point->next;
    return *this;
}

template<class T>
typename Container<T>::Iterator Container<T>::Iterator::operator++ (int) {
    Iterator temp = *this;
    if(point)
        point = point->next;
    return temp;
}

template<class T>
typename Container<T>::Node* Container<T>::Iterator::operator-> () const { return &point; } //testare

template<class T>
typename Container<T>::Node& Container<T>::Iterator::operator* () const { return *point; } //testare

//ridefinizionde dell'operatore << di Container
template<class T>
ostream& operator<<(ostream&, const Container<T>&);

template<class T>
ostream& operator<< (ostream& os, const Container<T>& c) {

    os << "(";
    Node<T>*n = c.first;

    for(; n !=0; n=n->next)
        os << *n <<" ";
    os<<")"<<endl;
    return os;
}

//ridefinizione dell'operatorre << di Nodo
template<class T>
ostream& operator<< (ostream& os, const Node<T>& n) {
    os << n->info;
    return os;
}

//metodi di container forniti all'utente
template<class T>
void Container<T>::Insert(const T t) { //gli indirizzi dei puntatori vengono ordinati in ordine crescente
    if(!first)
        first = new Node(t, nullptr);
    else
    {
        Node*prec = 0;
        Node*temp = first;

        while(temp && t >= temp->info)
        {
            prec = temp;
            temp = temp->next;
        }

        if(temp)
        {
            if(!prec)
                first = new Node(t, first); //in testa
            else
                prec->next = new Node(t, temp); //in mezzo
        }
        else
            prec->next = new Node(t, nullptr); //in coda
    }
}

template<class T>
void Container<T>::Remove(const T& t) {
    if(first)
    {
        Node*prec = nullptr;
        Node*temp = first;
        while(temp && t != temp->info)
        {
            prec = temp;
            temp = temp->next;
        }
        if(temp)
        {
            if(!prec)
                first = temp->next;
            else
                prec->next = temp->next;
            delete temp;
        }
    }
}

template<class T>
bool Container<T>::Search(const T& t) const { //trova l'indirizzo del puntatore passato come parametro
    if(!first) return false;

    bool trovato = false;
    Node*temp = first;

    while(temp && !trovato)
    {
        if(t == temp->info)
            trovato = true;
        else
            temp = temp->next;
    }
    return trovato;
}

template<class T>
unsigned long Container<T>::Size() const {
    if(!first) return 0;

    unsigned long conta = 0;
    Node*temp = first;
    while(temp)
    {
        conta++;
        temp = temp->next;
    }
    return conta;
}

template<class T>
bool Container<T>::Empty() const { return !first; }

template<class T>
void Container<T>::Clear() { destroy(first); first = nullptr; }

template<class T>
typename Container<T>::Iterator Container<T>::Begin() const {
    Iterator temp;
    temp.point = first;
    return temp;
}

template<class T>
typename Container<T>::Iterator Container<T>::End() const {
    Iterator temp;
    temp.point = 0;
    return temp;
}

#endif // CONTAINER_H
