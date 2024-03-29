# README

![Final](https://github.com/Ekelem/ICP_BlockEditor_2018/blob/master/doc/example.png)

Predmet: ICP

Projekt: Blokový Editor

Datum: 01.05.2018

Autori: Erik Kelemen(xkelem01), Attila Lakatos(xlakat01)

## Zadanie:
Navrhněte a implementujte aplikaci pro návrh a editaci blokových schémat.

Poznámka: Zadání definuje podstatné vlastnosti aplikace, které musí být splněny. Předpokládá se, že detaily řešení si doplní řešitelské týmy.
Týmy budou dvoučlenné, výjimečně může být tým i tříčlenný a bude řešit rozšířené zadání (viz níže). Jeden člen týmu je vedoucí a ten projekt odevzdá podle pokynů uvedených níže.

### Specifikace požadavků

#### Základní požadavky
       aplikace umožní vytvářet, editovat, ukládat a načítat bloková schémata
       každé schéma má svůj jedinečný název
       vytvořená schémata lze uložit a opětovně načíst
       schéma je složeno z bloků a propojů mezi bloky
#### Bloky
       každý blok má definované vstupní a výstupní porty
       s každým portem je spojen typ, který je reprezentován množinou dat v podobě dvojic název->hodnota; hodnota bude vždy typu double
       bloky je možné spojit pouze mezi výstupním a vstupním portem
       každý blok obsahuje výpočet (vzorce), které transformují hodnoty ze vstupních portů na hodnoty výstupních portů
#### Propojení mezi bloky
       systém kontroluje kompatibilitu vstupního a výstupního portu propoje (stejný typ dat)
       typ dat je přiřazen propoji automaticky podle spojených portů
#### Výpočet
       po sestavení (načtení) schématu je možné provést výpočet
       systém detekuje cykly v schématu; pokud jsou v schématu cykly, nelze provést výpočet
       systém požádá o vyplnění dat vstupních portů, která nejsou napojena a poté postupně provádí výpočty jednotlivých bloků podle definovaných vzorců v každém bloku
       při výpočtu se vždy zvýrazní blok, který je právě přepočítáván
       výpočet lze krokovat (jeden krok = přepočet jednoho bloku)
#### Další podmínky
       najetím myši nad propoj se zobrazí aktuální stav dat
       zvažte způsob jednoduchého rozšiřování systému o nové bloky a data

### Spustenie
    Projekt je možné preložiť pomocou make a následne spustiť pomocou make run
