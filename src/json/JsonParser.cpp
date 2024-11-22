#include "JsonParser.h"

String getJsonValue(const String &json, const char *key) {
    // Taille du buffer pour le parsing JSON (ajustez selon la taille de votre JSON)
    StaticJsonDocument<2048> doc;

    // Désérialisation du JSON
    DeserializationError error = deserializeJson(doc, json);

    // Vérifiez si la désérialisation a échoué
    if (error) {
        Serial.print("Erreur lors du parsing JSON: ");
        Serial.println(error.c_str());
        return ""; // Retourne une chaîne vide en cas d'erreur
    }

    // Récupérer la valeur associée à la clé
    const char *value = doc[key];

    // Si la clé n'existe pas, retourner une chaîne vide
    if (value == nullptr) {
        Serial.print("Clé introuvable: ");
        Serial.println(key);
        return "";
    }

    // Retourner la valeur sous forme de chaîne
    return String(value);
}