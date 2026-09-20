#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

extern long convertir_gini(
    float a,
    float b,
    float c,
    float d,
    float e,
    float f,
    float g,
    float h,
    float gini
);

struct Memoria {
    char datos[4096];
    size_t tamanio;
};

size_t guardar_respuesta(
    void *contenido,
    size_t tamanio,
    size_t cantidad,
    void *usuario
) {
    size_t total = tamanio * cantidad;

    struct Memoria *memoria = (struct Memoria *)usuario;

    memcpy(
        memoria->datos + memoria->tamanio,
        contenido,
        total
    );

    memoria->tamanio += total;
    memoria->datos[memoria->tamanio] = '\0';

    return total;
}

int main() {

    CURL *curl;
    CURLcode res;

    struct Memoria memoria;

    memoria.tamanio = 0;
    memoria.datos[0] = '\0';

    curl = curl_easy_init();

    if (!curl) {
        printf("Error iniciando CURL\n");
        return 1;
    }

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "https://api.worldbank.org/v2/country/ARG/indicator/SI.POV.GINI?date=2024&format=json"
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        guardar_respuesta
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &memoria
    );

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("Error consultando la API\n");
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_cleanup(curl);

    char *posicion = strstr(
        memoria.datos,
        "\"date\":\"2024\",\"value\":"
    );

    if (posicion == NULL) {
        printf("No se encontro el indice GINI\n");
        return 1;
    }

    posicion += strlen("\"date\":\"2024\",\"value\":");

    float gini = atof(posicion);

    long resultado = convertir_gini(
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        gini
    );

    printf("Indice GINI de Argentina: %.1f\n", gini);
    printf("GINI convertido a entero + 1: %ld\n", resultado);

    return 0;
}
