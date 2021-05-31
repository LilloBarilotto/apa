#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXREG 100
#define MAXSTRING 10000

char * cercaRegexpspecifica(char *src, char *regexp);
char *cercaRegexp(char *src, char *regexp, int lengthregexp);
int contareal(char *regexp);

int main() {
    char regexp[MAXREG];
    char input[MAXSTRING];
    char parola[MAXSTRING];

    char *s, *puntverifica;
    int i=0, lunghezzaregexp;

    printf("Inserisci la stringa di input che dovra' essere controllata:\n");
    fgets(input,MAXSTRING,stdin);
    s=input;

    printf("Inserisci la regexp:\n");
    fgets(regexp,MAXREG,stdin);

    lunghezzaregexp=contareal(regexp);

    while (sscanf(s, "%s%n", parola, &i)>0) {
        s = s+i; // oppure s = &s[i];

        puntverifica=cercaRegexp(parola, regexp, lunghezzaregexp);

        if(puntverifica!=NULL){
            printf("Nella parola %s ho trovato l'occorrenza '",parola);

            for(i=0; i<lunghezzaregexp; i++)
                printf("%c", *(puntverifica+i));
            printf("' \n\n");

            return 0;
        }
    }

    printf("Non e' stata trovata nessuna occorrenza mi spiace\n\n");

    return 0;
}

int contareal(char *regexp){ //mi conta quanto dovrà essere lunga la corrispondente sottostringa della regexp
    int i=0;

    while( *regexp!='\n'){
        if(*regexp=='\\')
            regexp++;
        else if(*regexp=='[')
            while(*regexp!=']')
                regexp++;

        regexp++;
        i++;
    }

    return i;
}

char *cercaRegexp(char *src, char *regexp, int lengthregexp){

    int lengthsrc=strlen(src), i=0;

    while(i<=lengthsrc-lengthregexp){
        if(cercaRegexpspecifica(src,regexp)!=NULL)
            return src;

        src++;
        i++;
    }
    return  NULL;
}

char * cercaRegexpspecifica(char *src, char *regexp){
    char * srcvalido=src;
    while( *regexp!='\n'){
        switch (*regexp) {
            case '\\':
                regexp++;

                if( (*regexp=='A' && isupper(*src)) || (*regexp=='a' && islower(*src))) {
                    src++;
                    regexp++;
                }
                else
                    return NULL;
                break;

            case '[':
                regexp++;

                if(*regexp=='^'){   //tutti quelli diversi da quel determinato valore char puntato da src
                    regexp++;

                    while(*regexp!=*src && *regexp!=']')        //se esco perche' sono arrivato a ']' significa che ha superato il controllo bene
                        regexp++;

                    if(*regexp!=']')
                        return NULL;
                    else
                        while(*regexp!=']')
                            regexp++;
                }
                else{
                    while(*regexp!=*src && *regexp!=']')
                        regexp++;

                    if(*regexp==']')
                        return NULL;
                    else
                        while(*regexp!=']')
                            regexp++;
                }
                src++;
                regexp++;

                break;

            case '.':
                regexp++;
                src++;

                break;

            default:
                if(*regexp==*src){
                    regexp++;
                    src++;
                }
                else
                    return NULL;
        }
    }
    return  srcvalido;
}