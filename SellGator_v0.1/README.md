# SellGator v0.1 (MVP)

## Application ERP en C++

Une première implémentation d'un ERP de vente côté console est disponible dans le dossier `cpp/`.

### Compilation

```bash
cd cpp
cmake -S . -B build
cmake --build build
```

L'exécutable est généré sous `build/sellgator_erp`.

### Exécution

```bash
./build/sellgator_erp
```

L'application propose un menu simple pour gérer les produits, clients et commandes.