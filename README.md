# 🚀 Projet M5Stack RFID & Dolibarr API

## 📌 Présentation du projet  

Ce projet a été réalisé dans le cadre d'un projet scolaire, en **équipe de 4 étudiants**, sur une durée de **3 mois**.  
L'objectif est de développer un **système de gestion d'inventaire RFID**, basé sur un **M5Stack Core S3**, capable de :

- Lire et écrire des données sur des **cartes RFID** (MIFARE 1K).
- Communiquer avec l’**API Dolibarr** pour récupérer des informations sur les produits.
- Piloter un **convoyeur motorisé** et des **servo-moteurs**.
- Offrir une interface web embarquée pour **configurer le WiFi** et l’**API Dolibarr** à distance.

## 🛠️ **Technologies utilisées**  

Le projet repose sur plusieurs technologies et bibliothèques :

| Technologie / Matériel | Utilisation |
|------------------------|-------------|
| **M5Stack Core S3** | Microcontrôleur principal |
| **ESP32 (WiFi)** | Connexion réseau et API |
| **RFID MFRC522** | Lecture/écriture des cartes MIFARE |
| **GoPlus2** | Gestion des servo-moteurs |
| **GRBL Module** | Pilotage du moteur pas-à-pas |
| **Dolibarr API** | Gestion de l’inventaire |
| **PlatformIO** | Environnement de développement |
| **ESPAsyncWebServer** | Interface web embarquée |
| **Preferences (ESP32)** | Sauvegarde des paramètres WiFi/API |

---

## 🎯 **Fonctionnalités principales**  

### 🔹 **1. Gestion du RFID**
✅ Lecture des **UIDs** et des **données enregistrées** sur les cartes RFID MIFARE 1K.  
✅ Écriture des **données** sur les cartes RFID via une page Web.  
✅ Mode **Lecture/Écriture** configurable via une interface Web.  

### 🔹 **2. Connexion WiFi et API Dolibarr**
✅ **Mode STA** : Connexion automatique au WiFi configuré.  
✅ **Mode AP** : Si aucun WiFi n’est configuré, le M5Stack crée un **point d’accès WiFi** pour la configuration.  
✅ Envoi de requêtes **GET/POST** vers l’API **Dolibarr** pour récupérer des informations produits et gérer les stocks.  

### 🔹 **3. Interface Web embarquée**  
✅ Page Web pour **configurer le WiFi** et l'**API Dolibarr** sans recompiler le code.  
✅ Changement du **mode RFID** (Lecture/Écriture).  
✅ Saisie des **données à écrire** sur les cartes RFID.  
✅ Affichage de l'**adresse IP** pour se connecter facilement.  

### 🔹 **4. Gestion du convoyeur et des moteurs**
✅ Pilotage d’un **moteur pas-à-pas** via **GRBL**.  
✅ Contrôle des **servo-moteurs** avec **GoPlus2**.  
✅ Association des **cartes RFID** à des **positions moteur** pour automatiser le tri des produits.  

---

## 🔧 **Installation et configuration**  

### 📥 **1. Prérequis**  
✅ **PlatformIO** installé sur **VS Code**  
✅ **M5Stack Core S3** connecté en USB  
✅ Un **serveur Dolibarr** fonctionnel avec l'API activée  

### 🛠️ **2. Installation du projet**  
```sh
git clone https://github.com/votre-repo/m5stack-rfid-dolibarr.git
cd m5stack-rfid-dolibarr