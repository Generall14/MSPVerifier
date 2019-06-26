#ifndef MSPVERIFIER_HPP
#define MSPVERIFIER_HPP

#include <QString>

namespace MSPV
{

    /**
     * Funkcja wszystkie wymagane czynności w symulacji, tj. pobieranie listy plików obiektem klasy FileList, wyciąganie z niej listy funkcji obiektem FunContainer, pobieranie spisu konwencji Convs oraz wykonywanie symulacji na obiekcie listy funkcji.
     * @param ewpFile - adres pliku projektu IAR *.ewp.
     * @param convsFile - adres pliku opisu konwencji wywołań.
     */
    void verify(QString ewpFile, QString convsFile, bool quiet=false);
}

#endif
