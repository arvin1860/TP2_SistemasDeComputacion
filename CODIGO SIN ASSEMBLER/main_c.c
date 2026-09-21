#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

long gini_mas_uno_c(double v);

struct buffer { char *datos; size_t largo; };

static size_t escribir(void *ptr, size_t size, size_t nmemb, void *userdata) {
    struct buffer *b = userdata;
    size_t total = size * nmemb;
    char *nuevo = realloc(b->datos, b->largo + total + 1);
    if (!nuevo) return 0;
    b->datos = nuevo;
    memcpy(b->datos + b->largo, ptr, total);
    b->largo += total;
    b->datos[b->largo] = '\0';
    return total;
}

int main(void) {
    const char *url = "https://api.worldbank.org/v2/country/ARG/indicator/SI.POV.GINI"
                      "?format=json&date=2011:2020&per_page=100";
    struct buffer b = { NULL, 0 };

    CURL *curl = curl_easy_init();
    if (!curl) return 1;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, escribir);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &b);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    CURLcode rc = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (rc != CURLE_OK || !b.datos) {
        fprintf(stderr, "Error de red: %s\n", curl_easy_strerror(rc));
        return 1;
    }

    const char *p = b.datos;
    while ((p = strstr(p, "\"date\":\"")) != NULL) {
        p += 8;
        int anio = atoi(p);
        const char *v = strstr(p, "\"value\":");
        if (!v) break;
        v += 8;
        if (strncmp(v, "null", 4) != 0) {          // saltear años sin dato
            double gini = strtod(v, NULL);
            printf("%d: Gini %.1f -> %ld\n", anio, gini, gini_mas_uno_c(gini));
        }
        p = v;
    }
    free(b.datos);
    return 0;
}
