# 🔐 PROTOCOLE DE CHIFFREMENT/DÉCHIFFREMENT

> Système de chiffrement avancé par substitution et inversion

---

## 🛠️ Fonctions Disponibles

| Fonction | Description | Paramètre d'entrée | Valeur de retour |
|----------|-------------|-------------------|------------------|
| `encrypt` | Chiffre un message en texte clair | message (str) - Texte à chiffrer | str - Message chiffré |
| `decrypt` | Déchiffre un message chiffré | message (str) - Texte chiffré | str - Message en texte clair |

---

## 🔄 Processus de Communication

### Étapes du processus :

1. **Chiffrement**
   - L'expéditeur utilise `encrypt()` pour chiffrer son message

2. **Transmission**
   - Le message chiffré est envoyé au destinataire

3. **Déchiffrement**
   - Le destinataire utilise `decrypt()` pour récupérer le message original

---

## 💡 Exemples d'Utilisation

### Exemple 1 :

```
Texte original : bonjour
        ↓ encrypt() ↓
Texte chiffré : ıͰͳ̅ͰΛͱ
        ↓ decrypt() ↓
Texte récupéré : bonjour
```

### Exemple 2 :

```
Texte original : aurevoir
        ↓ encrypt() ↓
Texte chiffré : ΛͱͰͰǯͮˌͱ
        ↓ decrypt() ↓
Texte récupéré : aurevoir
```

---

## 🔧 Caractéristiques du Chiffrement

### Propriétés générales

- **Type de chiffrement** : Chiffrement par substitution avec inversion de chaîne
- **Réversibilité** : Totalement réversible avec la fonction `decrypt()`
- **Longueur du message** : Conservée (même nombre de caractères)
- **Caractères supportés** : Tous les caractères ASCII

### Processus de chiffrement

**Étapes :**
1. Inversion de la chaîne de caractères
2. Application de la clé de chiffrement à chaque caractère

### Clé de chiffrement

Décalage calculé par la formule suivante :

```
K = 100⋅cos²(4π) + 60⋅tan(6π) + 90⋅sin(2π) + 80⋅cos(0) + 10⋅sin²(6π) + (167.5 − 60⋅tan(6π)) + 0,5
```

---

## 📨 Messages Reçus - Analyse

### Message Type 1

```
Message reçu en clair : I_need_help_to_level_up_to_X_with_Y
Type de communication : Requête d'assistance pour évolution de statut
Note : X = incantation et Y = food
```

### Message Type 2

```
Message reçu en clair : I_am_starting_to_play
Type de communication : Notification de changement d'état
```

---

## 🔄 Flux de Données

| Entrée | Fonction | Sortie |
|--------|----------|--------|
| Message en texte clair | `encrypt()` | Message chiffré |
| Message chiffré | `decrypt()` | Message en texte clair |

---

## 📋 Spécifications Techniques

### Algorithme

1. **Inversion** : La chaîne d'entrée est inversée
2. **Substitution** : Chaque caractère subit un décalage basé sur la clé K
3. **Réversibilité** : Le processus inverse permet de retrouver le texte original

### Sécurité

- Chiffrement symétrique
- Clé de chiffrement fixe basée sur des constantes mathématiques
- Protection par obscurité du mécanisme d'inversion

### Performance

- **Complexité temporelle** : O(n) où n est la longueur du message
- **Complexité spatiale** : O(n) pour le stockage du message transformé
- **Compatibilité** : Tous environnements supportant les caractères ASCII

---

## 🔍 Notes d'Implémentation

### Utilisation recommandée

```python
# Chiffrement
message_original = "bonjour"
message_chiffre = encrypt(message_original)
# Résultat : "ıͰͳ̅ͰΛͱ"

# Déchiffrement
message_recupere = decrypt(message_chiffre)
# Résultat : "bonjour"
```

### Gestion des erreurs

- Vérifier que l'entrée est une chaîne de caractères valide
- S'assurer que tous les caractères sont dans la plage ASCII supportée
- Valider l'intégrité du message avant déchiffrement

---

*Protocole de chiffrement - Version 1.0*