#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

int main()
{
    /* ---------- INIZIANDO VARIABLES ---------- */

    /*  
       1. cliente es un descriptor de archivo para almacenar los valores
        devuelto por la llamada del sistema socket y la aceptación
        llamada del sistema.

       2. portNum es para almacenar el número de puerto en el que
        el acepta conexiones.

       3. isExit es una variable bool que se usará para
        terminar el ciclo

       4. El cliente lee los caracteres del socket.
        conexión en una variable dinámica (búfer).

       5. Un sockaddr_in es una estructura que contiene una direción de internet.
        Esta estructura ya está definida en netinet/in.h, por lo que
        no necesitamos declararlo de nuevo.

        DEFINICION:

        struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };

        6. serv_addr contendrá la dirección del servidor

    */

    int client;
    int portNum = 1500; // Tenga en cuenta que el número de puerto es el mismo, tanto para el cliente como para el servidor
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";

    struct sockaddr_in server_addr;

    client = socket(AF_INET, SOCK_STREAM, 0);

    /* ---------- ESTABLECIENDO CONECCION DE SOCKET ----------*/
    /* --------------- socket() function --------------------*/

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    /*
        La función socket() crea un nuevo socket.
         Se necesitan 3 argumentos.

             a. AF_INET: dirección de dominio del socket.
             b. SOCK_STREAM: Tipo de socket, una toma de corriente en
             qué caracteres se leen en un flujo continuo (TCP)
             C. El tercero es un argumento de protocolo: siempre debe ser 0. El
             OS elegirá el protocolo más apropiado.

             Esto devolverá un número entero pequeño y se usa para todos
             referencias a este socket. Si la llamada del socket falla,
             devuelve -1.

    */

    cout << "\n=> Socket client has been created..." << endl;
    
    /* 
         La variable serv_addr es una estructura de sockaddr_in.
         sin_family contiene un código para la familia de direcciones.
         Siempre debe establecerse en AF_INET.

         htons() convierte el número de puerto del orden de bytes del host
         a un número de puerto en orden de bytes de red.

    */

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNum);

    // esta función devuelve devuelve 1 si la IP es válida
    // y 0 si no es válido
    // inet_pton convierte IP en paquetes
    // inet_ntoa convierte los paquetes de vuelta a IP
    //inet_pton(AF_INET, ip, &server_addr.sin_addr);

    /*if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server " << inet_ntoa(server_addr.sin_addr) << " with port number: " << portNum << endl;*/


    /* ---------- CONECTANDO EL SOCKET ---------- */
    /* ---------------- connect() ---------------- */

    if (connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
        cout << "=> Connection to the server port number: " << portNum << endl;

    /* 
         El cliente llama a la función de conexión para
         establecer una conexión con el servidor. Se necesita
         tres argumentos, el descriptor de archivo de socket, el
         dirección del host al que se quiere conectar
         (incluyendo el número de puerto), y el tamaño de este
         dirección.

         Esta función devuelve 0 en caso de éxito y -1
         si falla.

         Tenga en cuenta que el cliente necesita saber el número de puerto de
         el servidor pero no su propio número de puerto.
    */

    cout << "=> Awaiting confirmation from the server..." << endl; //line 40
    recv(client, buffer, bufsize, 0);
    cout << "=> Connection confirmed, you are good to go...";

    cout << "\n\n=> Enter # to end the connection\n" << endl;

    // Una vez que llega aquí, el cliente puede enviar un mensaje primero.

    do {
        cout << "Client: ";
        do {
            cin >> buffer;
            send(client, buffer, bufsize, 0);
            if (*buffer == '#') {
                send(client, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != 42);

        cout << "Server: ";
        do {
            recv(client, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }

        } while (*buffer != 42);
        cout << endl;

    } while (!isExit);

    /* ---------------- CLOSE CALL ------------- */
    /* ----------------- close() --------------- */

    /* 
         xUna vez que el servidor presiona # para finalizar la conexión,
         el bucle se romperá y cerrará el servidor,
         conexión de socket y la conexión del cliente.
    */

    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}