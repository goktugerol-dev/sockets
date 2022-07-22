#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    /* ---------- INITIALIZING VARIABLES ---------- */

    /*  
       1. cliente/servidor son dos descriptores de archivo
        Estas dos variables almacenan los valores devueltos.
        {or la llamada al sistema socket y la llamada al sistema accept.

       2. portNum es para almacenar el número de puerto en el que
        el acepta conexiones

       3. isExit es una variable bool que se usará para
        terminar el ciclo

       4. El servidor lee los caracteres del socket.
        Conexión en una variable dinámica (búfer).

       5. Un sockaddr_in es una estructura que contiene una dirección de internet
        Esta estructura ya está definida en netinet/in.h, por lo que
        no necesitamos declararlo de nuevo.

        DEFICION:

        struct sockaddr_in
        {
          short   sin_family;
          u_short sin_port;
          struct  in_addr sin_addr;
          char    sin_zero[8];
        };

        6. serv_addr contendrá la dirección del servidor

        7. socklen_t es un tipo intr de ancho de al menos 32 bits


    */
    int client, server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];

    struct sockaddr_in server_addr;
    socklen_t size;

    /* ---------- ESTABLECIMIENTO DE LA CONEXIÓN SOCKET ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) 
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    /*
        La función socket() crea un nuevo socket.
         Se necesitan 3 argumentos,

            a. AF_INET: dirección de dominio del socket.
            b. SOCK_STREAM: Tipo de socket. Un socket de corriente en
             qué caracteres se leen en un flujo continuo (TCP)
            c. El tercero es un argumento de protocolo: siempre debe ser 0. El
             OS elegirá el protocolo más apropiado.

            Esto devolverá un número entero pequeño y se usa para todos
             referencias a este socket. Si la llamada del socket falla,
             devuelve -1.

    */

    cout << "\n=> Socket server has been created..." << endl;

    /* 
       La variable serv_addr es una estructura de sockaddr_in.
         sin_family contiene un código para la familia de direcciones.
         Siempre debe establecerse en AF_INET.

       INADDR_ANY contiene la dirección IP del host. Para
         código del servidor, esta siempre será la dirección IP de
         la máquina en la que se ejecuta el servidor.

        htons() convierte el número de puerto del orden de bytes del host
         a un número de puerto en orden de bytes de red.

    */

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);

    /* ---------- VINCULAR EL SOCKET ---------- */
    /* ---------------- bind() ---------------- */


    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
    {
        cout << "=> Error de vincular la conneción, el socket ya esta establecido..." << endl;
        return -1;
    }

    /* 
         La llamada al sistema bind() vincula un socket a una dirección, 
         en este caso, la dirección del host actual y el número de puerto
         en el que se ejecutará el servidor. Se necesitan tres argumentos,
         el descriptor de archivo de socket. El segundo argumento es un puntero.
         a una estructura de tipo sockaddr, esta debe ser convertida a
         el tipo correcto.
    */

    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    /* ------------- ESCUCHANDO LLAMADA ------------- */
    /* ---------------- listen() ------------------- */

    listen(client, 1);

    /* 
        La llamada al sistema de escucha permite que el proceso escuchar
        en el socket para las conexiones.

        El programa permanecerá inactivo aquí si no hay conexiones entrantes.

         El primer argumento es el descriptor del archivo de socket,
         y el segundo es el tamaño para el número de clientes
         es decir, el número de conexiones que el servidor puede
         manejar mientras el proceso está manejando un determinado
         conexión. El tamaño máximo permitido por la mayoría
         sistemas es 5.

    */

    /* ------------- ACCEPTANDO CLIENTES  ------------- */
    /* ----------------- listen() -------------------- */

    /* 
        La llamada al sistema accept() hace que el proceso se bloquee
         hasta que un cliente se conecta al servidor. Así, se despierta
         iniciar el proceso cuando una conexión de un cliente ha sido
         establecido con éxito. Devuelve un nuevo descriptor de archivo,
         y toda la comunicación sobre esta conexión debe hacerse
         utilizando el nuevo descriptor de archivo. El segundo argumento es un
         puntero de referencia a la dirección del cliente en el otro
         final de la conexión, y el tercer argumento es el tamaño
         de esta estructura.
    */

    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // primero checa si es valido o no.
    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server > 0) 
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;

        /* 
           Tenga en cuenta que solo llegaríamos a este punto después de un
             El cliente se ha conectado con éxito a nuestro servidor.
             Esto se lee desde el socket. Tenga en cuenta que la read()
             se bloqueará hasta que haya algo para leer
             en el socket, es decir, después de que el cliente haya ejecutado un
             send().

           Leerá el número total de caracteres
             en el socket o 1024
        */

        cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

        /* 
           Una vez establecida la conexión, ambos extremos
             puede leer y escribir en la conexión. Naturalmente,
             todo lo escrito por el cliente será leído por el
             servidor, y todo lo escrito por el servidor será
             leído por el cliente.
        */

        /* ---------------- CLOSE CALL ------------- */
        /* ----------------- close() --------------- */

        /* 
             Una vez que el servidor presiona # para finalizar la conexión,
             el bucle se romperá y cerrará el servidor
             conexión de socket y la conexión del cliente.
        */

        // inet_ntoa convierte los datos del paquete a IP, que se tomó del cliente
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;
}