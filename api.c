#include <stdio.h>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            "https://api.open-meteo.com/v1/forecast?latitude=-31.42&longitude=-64.18&current=temperature_2m"
        );

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("Error al consultar la API\n");
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
