
# Ohjelmoinnin jatkokurssi

Projektiloki Turun ammattikorkeakoulun "Ohjelmoinnin jatkokurssi" opintojakson osan projektityötä varten



## Projektin selostus


## Laitteisto

Käytössä on ciseco RasWIK kitti:
- Raspberry PI + Slice of Radio -moduuli
- XinoRF

- 2 x Astrosym Minebea 17PM - MO44-01V

### Toimilaitteet

Kaksi kappaletta toisistaan riippumatonta Astrosym Minebea 17PM-M44-01V bipolaari askelmoottoria.
Moottorin specifikaatio:

- Ulko-osan halkaisija: 1.7''
- Tyyppi: precision, hybrid
- Askelkoko: 1.8 astetta
- Rakenne 2 & 4 vaihe hybridi
- 44 Käämiä
- Versio 01

[Minebea stepper motor number decoding](http://www.theprojectasylum.com/electronicsprojects/componentdata/minebeasteppedatacirca1999.html)

## Ohelmisto

### Priorteetit

- Käyttöliittymä
    - Konsoli kontrolleri
        - **USB HID laiteen lukeminen**
    - Telemetrian piirtäminen
        - jako omaan prosessiinse prosesseihin
        - zmq sokettejen käsittelyä varten
        - piirtämis framework: QT, python

- Khden suuntanen kommunikointi protokolla
    - **Uart raspberry PIllä kuntoon**
    - Häiriön poisto 
        - crc, hamming?
- moottorin ohjauselektroniikka
- rakennus tekniikka
    * rakennus matrialit
    * **renkaat**
- botin dynaaminen mallinnus
- mitä sensoreita on käytössä
    - pallohiirien tulkitseminen AVRllä
- Botin ohjaus
    - **tankki ohjaus**
    - vakionopeudensääniin
    - autonomia, tekoäly
- Teholähde botille
    - moottorieden tarve
    - 9V patteri?

## Arkkitehtuuri
