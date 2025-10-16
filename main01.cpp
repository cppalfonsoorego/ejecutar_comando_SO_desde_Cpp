#include <iostream>
#include <cstdio>
#include <string>
#include <sstream> // Para std::stringstream

std::string ejecutar_y_capturar_dinamico(const std::string& comando) {
    // Usaremos un tamaño de búfer razonable (4KB) SOLO para la lectura temporal.
    const int BUFSIZE_TEMP = 4096;
    char buffer[BUFSIZE_TEMP];
    std::string resultado_completo = "";

    FILE* pipe = popen(comando.c_str(), "r");

    if (!pipe) {
        throw std::runtime_error("No se pudo ejecutar el comando: " + comando);
    }

    try {
        // En lugar de depender de una sola llamada a fgets, 
        // leemos en bloques y concatenamos. std::string crece dinámicamente.
        while (fgets(buffer, BUFSIZE_TEMP, pipe) != NULL) {
            // El resultado completo (std::string) se encarga de la asignación
            // dinámica de memoria para acomodar toda la salida.
            resultado_completo += buffer;
        }
    } catch (...) {
        pclose(pipe); // Aseguramos que se cierre incluso si hay excepción
        throw;
    }

    pclose(pipe);
    return resultado_completo;
}

int main() {
    std::string comando = "ls -R /etc/ 2> /dev/null | grep \"\\.conf\" | wc -l"; 
    // Este comando podría generar una salida muy grande

    try {
        std::string salida = ejecutar_y_capturar_dinamico(comando);
        
        std::cout << "--- Salida Completa (Asignación Dinámica) ---\n";
        // Si la salida es muy larga, imprimimos solo una parte para no saturar la terminal
        if (salida.length() > 2048) {
            std::cout << salida.substr(0, 2048) << "\n... [Salida Truncada para mostrar] ...\n";
        } else {
            std::cout << salida;
        }
        // std::cout << "---------------------------------------------\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}