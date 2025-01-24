#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <ArduinoJson.h>
#include <string.h>

// Fonction pour extraire une valeur spécifique d'un JSON
String getJsonValue(const String &json, const char *key);

#endif // JSON_PARSER_H