# RPi-LedSPI
Raspberry Pi LED SPI

# Integrazione di Super Led APA su piattaforma Raspberry PI
### Fase 1 - Raccolta documentazione su Raspberry e SPI
 - Understanding SPI on Raspberry -> WiringPi
 - https://projects.drogon.net/raspberry-pi/wiringpi/spi-library/

WiringPi è una libreria ad-hoc per Raspberry PI che si basa su chiamate di Sistema native Linux. In particolare la libreria dedicata all’SPI interagisce con i device nativi SPI della Raspberry PI.
I driver kernel della periferica SPI forniscono accesso all'interfaccia SPI BCM2835.

In comune con molti altri driver di periferica rende disponibile dei files nella cartella /dev/ del sistema che permettono la scrittura e la lettura su essi con comandi ioctl. Il modulo HiPi::Device::SPI supporta la comunicazione con i dispositivi SPI utilizzando il driver del kernel.
Si possono avere i driver di periferica caricati all'avvio del sistema (nel caso mancassero), come descritto nella seguente guida http://raspberrypi.znix.com/hipidocs/topic_spidev.htm.

È anche possibile caricare i driver di periferica in modo dinamico con i comandi:
- modprobe spi_bcm2708
- modprobe spidev

Il vantaggio di caricamento all'avvio di questi moduli, è che si possono configurare i driver per consentire agli utenti non root di accedere ai file di dispositivo. Queste impostazioni di configurazione si applicano anche quando si utilizza modprobe ma il processo avrebbe bisogno dei permessi di root.

Il responsabile del dispositivo su Linux si chiama udev e si possono fornire regole per udev che controllano i permessi sui file di periferica che vengono creati.
I driver del kernel in Raspbian forniscono l'accesso alle periferiche SPI andando a modificare un apposito file chiamato raspi-blacklist.conf. Commentando la riga blacklist spi-bcm2708 (dalla guida) si abilita la periferica voluta:
```sh
$ cat /etc/modprobe.d/raspi-blacklist.conf
```
```sh
#blacklist spi and i2c by default (many users don't need them)
#blacklist spi-bcm2708
blacklist i2c-bcm2708
```

Il Raspberry Pi GPIO offre 2 SPI selezionabili con gli appositi pin e il driver di dispositivo funziona fornendo un file diverso per ogni selezione. 
```sh
static char *spiDev0 = "/dev/spidev0.0" ;
static char *spiDev1 = "/dev/spidev0.1" ;
```

### Fase 2 - Test dell’interfaccia SPI
Si è settata l’interfaccia SPI tramite la libreria sopra descritta in modo da mantenere sia un buono strato applicativo ma anche garantendo un interfacciamento diretto alle componenti core di Linux.

#####WIRINGPI
La libreria WiringPi è facilmente scaricabile da github.

Se non si dispone di GIT installato, sotto una delle versioni Debian (per esempio Raspbian), è possibile installarlo con:
```sh
sudo apt-get install git-core
```

Se si ottengono degli errori, assicurarsi che la raspberry sia aggiornata con le ultime versioni di Raspbian:
```sh
sudo apt-get update
sudo apt-get upgrade
```

Per ottenere WiringPi usando GIT:

```sh
git clone git: //git.drogon.net/wiringPi
```

Per accedere al contenuto della cartella scaricata invece e installare la libreria dare i seguenti comandi:
```sh
cd wiringPi
./build
```

Il nuovo script build compila e installa tutto per voi.

WiringPi include una libreria che può rendere più facile l'utilizzo dell'interfaccia SPI del Raspberry Pi.

In primo luogo, è necessario utilizzare l'utility GPIO per caricare i driver SPI nel kernel:
```sh
gpio load spi
```
Se avete bisogno di una dimensione di buffer maggiore di 4KB, è possibile specificare la dimensione (in KB) sulla riga di comando:
```sh
gpio load spi 100
```
assegnerà un buffer  di 100 KB. (Non dovrebbe essere necessario questo. Di default è più che sufficiente per la maggior parte delle applicazioni).

Per utilizzare la libreria SPI, è necessario includere la libreria nel vostro programma:
```sh
#include <wiringPiSPI.h>
```

######Funzioni disponibili:

```sh
int wiringPiSPISetup (int canale, velocità int);
```
Questo è il modo per inizializzare un canale (Pi L'ha 2 canali, 0 e 1). Il parametro velocità è un numero intero compreso tra 500.000 e 32.000.000 attraverso rappresenta la velocità di clock SPI in Hz.

Il valore restituito è il Linux file descrittore per il dispositivo, o -1 in caso di errore. Se è avvenuto un errore, è possibile utilizzare lo standard errno variabile globale per capire perché.
```sh
int wiringPiSPIDataRW (int canale, unsigned char * dati, int len);
```
Esegue una scrittura simultanea / transazione leggere sul bus SPI selezionato. I dati che era nel buffer vengono sovrascritti dai dati restituiti dal bus SPI.

###I programmi devono essere linkati con -lwiringPi.
<br>

##Codice di prova
```sh
#include <wiringPiSPI.h>
#include <stdio.h>

int main(void)
{
    int chan=1;
    int speed=1000000;

    if( wiringPiSPISetup (chan, speed)==-1)
    {
    	printf("Could not initialise SPI\n");
       return(0);
    }
     
    printf("When ready hit enter.\n");
    (void) getchar();// remove the CR
    unsigned char buff[100];

    while (1)
    {
        printf("Input a string, Input 0 to finish : ");
        gets (buff);
        if(buff[0]=='0')
        {
            break;
        }
        else
        {
            int ret=wiringPiSPIDataRW (chan,buff,100) ;
            printf ("%s \n", buff);
        } 
    } 
}
```
Per compilare il sorgente basta dare il seguente comando:
```sh
gcc –o prova prova.c –lwiringPi
```





