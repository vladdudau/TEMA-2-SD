#include <bits/stdc++.h>
using namespace std;

///Nodul unui Treap
struct Treap
{
    int cheie, prioritate;
    Treap *vec_stanga, *vec_dreapta;
};

Treap *RotireLaDreapta(Treap *y)
{
    Treap *x = y->vec_stanga,  *T2 = x->vec_dreapta;

    /// Realizeaza rotatia
    x->vec_dreapta = y;
    y->vec_stanga = T2;

    /// Returneaza noul treap
    return x;
}

Treap *RotireLaStanga(Treap *x)
{
    Treap *y = x->vec_dreapta, *T2 = y->vec_stanga;

    ///Realizaeza rotatia
    y->vec_stanga = x;
    x->vec_dreapta = T2;

    ///Returneaza noul treap
    return y;
}

/// Ma ajuta sa adaug o noua cheie
Treap* NewNod(int cheie)
{
    Treap* temporar = new Treap;
    temporar->cheie = cheie;
    temporar->prioritate = rand()%100;
    temporar->vec_stanga = temporar->vec_dreapta = NULL;
    return temporar;
}

///valoarae minima din Treap
int ValoareMinima(Treap* treap)
{
    if(treap==NULL)
        return INT_MAX;
    Treap* copie=treap;

///Cauta frunza cea mai din stanga
while (copie->vec_stanga != NULL)
{
    copie = copie->vec_stanga;
}
return(copie->cheie);
}

///functie care cauta valoarea maxima din Treap
int ValoareMaxima(Treap* treap)
{
    ///Caz de baza
    if (treap == NULL)
    return INT_MIN;

    /// Returneaza maximul a 3 valori
    /// 1) Cheia Radacinii 2) Maximul din subarborele din stanga
    /// 3) Maximul din subarborele din dreapta
    int Max1 = treap->cheie;
    int Max2 = ValoareMaxima(treap->vec_stanga);
    int Max3 = ValoareMaxima(treap->vec_dreapta);
    if (Max2 > Max1)
    Max1 = Max2;
    if (Max3 > Max1)
    Max1 = Max3;
    return Max1;
}

///Gaseste predecesorul unei chei
void GasestePredecesor(Treap* treap, Treap*& predecesor, int cheie)
{
    ///Cazul de baza
    if (treap == NULL ||ValoareMinima(treap)>=cheie)  return ;

    ///Daca cheia e in radacina
    if (treap->cheie == cheie)
    {
        ///Valoarea maxima din subarborele din stanga este predecesor
        if (treap->vec_stanga != NULL)
        {
            Treap* temporar = treap->vec_stanga;
            while (temporar->vec_dreapta)
                temporar = temporar->vec_dreapta;
            predecesor = temporar ;
        }
        return;
    }

    ///Daca cheia din radacina e mai mare decat cheia introdusa, cautam in subarborele din dreapta
    if (treap->cheie > cheie)
    {
        GasestePredecesor(treap->vec_stanga,predecesor, cheie) ;
    }
    else
    {
        predecesor=treap;
        GasestePredecesor(treap->vec_dreapta,predecesor, cheie) ;
    }
}

///Gaseste succesorul unei chei
void GasesteSuccesor(Treap* treap, Treap*& succesor, int cheie)
{
    ///Caz de baza
    if (treap == NULL ||ValoareMaxima(treap)<=cheie)  return ;

    ///Daca cheia se afla in radacina
    if (treap->cheie == cheie)
    {

        if (treap->vec_dreapta != NULL)
        {
            Treap* temporar = treap->vec_dreapta ;
            while (temporar->vec_stanga)
                temporar = temporar->vec_stanga ;
            succesor = temporar ;
        }
        return ;
    }

    if (treap->cheie > cheie)
    {
        succesor = treap ;
        GasesteSuccesor(treap->vec_stanga,succesor, cheie) ;
    }
    else
    {
        GasesteSuccesor(treap->vec_dreapta,succesor, cheie) ;
    }
}

///cauta o cheie in Treap
Treap* Cauta(Treap* treap, int cheie)
{
    /// Cazul de baza, treapul e null sau cheia e in radacina
    if (treap == NULL || treap->cheie == cheie)
       return treap;

    /// Cheia este mai mare decat cheia radacinii
    if (treap->cheie < cheie)
       return Cauta(treap->vec_dreapta, cheie);

    /// Cheia este mai mica decat cheia radacinii
    return Cauta(treap->vec_stanga, cheie);
}

///Inserare in Treap
Treap* Insereaza(Treap* treap, int cheie)
{
    /// Daca treapul e null creez un nou nod si l returnez
    if (!treap)
        return NewNod(cheie);

    /// Daca cheia este mai mica decat cheia radacinii
    if (cheie <= treap->cheie)
    {
        /// Inserez in subarborele din stanga
        treap->vec_stanga = Insereaza(treap->vec_stanga, cheie);

        if (treap->vec_stanga->prioritate > treap->prioritate)
            treap = RotireLaDreapta(treap);
    }
    else  /// Daca cheia este mai mare
    {
        /// Inserez in subarborele din dreapta
        treap->vec_dreapta = Insereaza(treap->vec_dreapta, cheie);

        if (treap->vec_dreapta->prioritate > treap->prioritate)
            treap = RotireLaStanga(treap);
    }
    return treap;
}

///Stergerea unui element din Treap
Treap* StergeNod(Treap* treap, int cheie)
{
    if (treap == nullptr)
        return treap;

    if (cheie < treap->cheie)
        treap->vec_stanga = StergeNod(treap->vec_stanga, cheie);
    else if (cheie > treap->cheie)
        treap->vec_dreapta = StergeNod(treap->vec_dreapta, cheie);

    else if (treap->vec_stanga == NULL)
    {
        Treap *temporar = treap->vec_dreapta;
        delete(treap);
        treap = temporar;  /// Fac copilul din dreapta sa fie radacina
    }


    else if (treap->vec_dreapta == NULL)
    {
        Treap *temp = treap->vec_stanga;
        delete(treap);
        treap = temp;  ///fac copilul din stanga sa fie radacina
    }

    /// Daca cheia este in radacina si left si right nu sunt nule
    else if (treap->vec_stanga->prioritate < treap->vec_dreapta->prioritate)
    {
        treap = RotireLaStanga(treap);
        treap->vec_stanga = StergeNod(treap->vec_stanga, cheie);
    }
    else
    {
        treap = RotireLaDreapta(treap);
        treap->vec_dreapta = StergeNod(treap->vec_dreapta, cheie);
    }

    return treap;
}

///Afiseaza arborele impreuna cu copiii sai
void AfisareInOrdine(Treap* treap)
{
    if (treap)
    {
        AfisareInOrdine(treap->vec_stanga);
        cout << "cheie: "<< treap->cheie << " | prioritate: "
            << treap->prioritate;
        if (treap->vec_stanga)
            cout << " | copilul din stanga: " << treap->vec_stanga->cheie;
        if (treap->vec_dreapta)
            cout << " | copilul din dreapta: " << treap->vec_dreapta->cheie;
        cout << endl;
        AfisareInOrdine(treap->vec_dreapta);
    }
}

void AfisareInOrdineCrescatoare(Treap* treap)
{
     if (treap == NULL)
        return;
    if (treap != NULL)
    {
        AfisareInOrdineCrescatoare(treap->vec_stanga);
        cout<<treap->cheie<<"  ";
        AfisareInOrdineCrescatoare(treap->vec_dreapta);
    }
}

int main()
{
    srand(time(NULL));
    struct Treap *treap = NULL;
    int alegere, valoare;
    bool gasit=false;
    while (1)
    {
        gasit=false;
        cout<<endl<<"----------------------------"<<endl;
        cout<<endl<<"Operatiile pe Treap"<<endl;
        cout<<endl<<"----------------------------"<<endl;
        cout<<"1.Insereaza element "<<endl;
        cout<<"2.Sterge element "<<endl;
        cout<<"3.Afiseaza elementul minim"<<endl;
        cout<<"4.Afiseaza elementul maxim"<<endl;
        cout<<"5.Afiseaza predecesorul"<<endl;
        cout<<"6.Afiseaza succesorul"<<endl;
        cout<<"7.Parcurgere in ordine afisand si copilul din stanga sau dreapta"<<endl;
        cout<<"8.Afisare in ordine crescatoare"<<endl;
        cout<<"9.Iesire"<<endl;
        cout<<"Introdu alegerea: ";
        cin>>alegere;
        cout<<endl;
        switch(alegere)
        {
        case 1:
            {
            cout<<"Introdu valoarea care trebuie inserata : ";
            cin>>valoare;
            treap=Insereaza(treap, valoare);
            break;
            }
        case 2:
            {
            if (treap == nullptr)
            {
                cout<<"Treap-ul este gol, nu ai ce sterge."<<endl;
                continue;
            }
            cout<<"Introdu numarul pe care vrei sa-l stergi : ";
            cin>>valoare;
            Treap *cauta=Cauta(treap,valoare);
            if(cauta!=nullptr)
                gasit = true;
            else
                cout<<"Elementul nu a fost gasit."<<endl;
            treap=StergeNod(treap, valoare);
            if (cauta!=nullptr && gasit)
                cout<<"Element Deleted"<<endl;
            break;
            }
        case 3:
            {
                int mini=ValoareMinima(treap);
                if(mini==INT_MAX)
                    cout<<"Treap-ul este vid"<<endl;
                else
                    cout<<"Valoarea minima este: "<<mini;
                break;
            }
        case 4:
            {
                int maxi=ValoareMaxima(treap);
                if(maxi==INT_MIN)
                    cout<<"Treap-ul este vid"<<endl;
                else
                    cout<<"Valoarea minima este: "<<maxi;
                break;
            }
        case 5:
            {
                Treap* predecesor=NULL;
                int val;
                cout<<"Dati valoarea pentru care vreti sa gasiti un predecesor: ";
                cin>>val;
                GasestePredecesor(treap,predecesor,val);
                if(predecesor==NULL)
                    cout<<"Nu exista un predecesor pentru valoarea citita in treap."<<endl;
                else
                {
                    cout<<"Predecesorul valorii citite este "<<predecesor->cheie<<endl;
                }
                break;
            }
        case 6:
            {
                Treap* succesor=NULL;
                int val;
                cout<<"Dati valoarea pentru care vreti sa gasiti un succesor: ";
                cin>>val;
                GasesteSuccesor(treap,succesor,val);
                if(succesor==NULL)
                    cout<<"Nu exista un succesor pentru valoarea citita in treap."<<endl;
                else
                {
                    cout<<"Succesorul valorii citite este "<<succesor->cheie<<endl;
                }
                break;
            }
        case 7:
            {
            if (treap == nullptr)
            {
                cout<<"Treap-ul este gol, inseareaza un element mai intai: "<<endl;
                continue;
            }
            cout<<"Parcurgere in ordine afisand si copilul din stanga sau dreapta : "<<endl;
            AfisareInOrdine(treap);
            cout<<endl;
            break;
            }
        case 8:
            {
            if (treap == nullptr)
            {
                cout<<"Treap-ul este gol"<<endl;
                continue;
            }
            cout<<"Afiseaza elementele in ordine crescatoare :"<<endl;
            AfisareInOrdineCrescatoare(treap);
            cout<<endl;
            break;
            }
        case 9:
            {
            exit(1);
            break;
            }
        default:
            cout<<"Alegere gresita"<<endl;
        }
    }

    return 0;
}
