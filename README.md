

![logo](https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/097acf7a-c695-4cca-8fe9-1894ff191e88)

Progetto creato per il corso di [Sistemi Embedded](https://gitlab.di.unimi.it/sistemiembedded) dell'Università Statale di Milano
_______________________________
### hardware utilizzato
  - **attuatori**:
    - [led rgb](https://www.amazon.it/Youmile-emissione-Tri-Color-trasparenti-potrebbero/dp/B0897L6TZ8)
    - [servomotore](https://www.amazon.it/Aihasd-28YBJ-48-4-Phase-Stepper-Arduino/dp/B00U744GJU)
  - **sensori**:
    - [pulsante con leva](https://www.amazon.it/GOLRISEN-Interruttore-Finecorsa-Attuatore-Momentaneo/dp/B07F9QKTQQ)
    - [sensore di distanza a ultrasuoni](https://www.amazon.it/CML-Modulo-Ultrasuoni-Distanza-Trasduttore-Misurazione/dp/B00JB1C142)
    - [cella di carico (1 kg)](https://www.amazon.it/elettronica-portatile-sensore-HX711-pesatura-Raspberry/dp/B076PYX5DW)

### funzionamento in breve
Il progetto prevede la realizzazione di un distributore di croccantini che, durante l'arco della giornata, riempia la ciotola del cibo in modo autonomo. Il sistema utilizza una cella di carico per registrare il peso del cibo presente nella ciotola e attivare un servomotore quando necessario, garantendo così un rifornimento costante. Inoltre, un sensore di distanza a ultrasuoni controlla il livello di croccantini nel contenitore, segnalando quando la riserva sta per esaurirsi.  
Il led RGB segnala diversi stati:
  - **in blu**, il contenitore non ha più abbastanza cibo da fornire
  - **in rosso**, il coperchio del contenitore è aperto per ricaricare la scorta di croccantini (in questa fase tutto il sistema è momentaneamente sospeso)
  - **in verde**, il servomotore sta fornendo cibo
_______________________________
### costruzione
- questo è lo schema del plexiglass (da 4 mm) da tagliare, con tutte le misure che ho usato per costruire la struttura. Successivamente con una pistola termica ho modellato il plexiglass creando delle curve di 90° per ogni lato da piegare.
  <br><br><img src="https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/c57f1120-a306-4538-b359-df634e556f45" alt="misure plexiglass" width="500"/><br><br>
  **1° N.B.** le proporzioni nell'immagine non sono quelle reali. Serve solo per avere le indicazioni sulle misure<br>
  **2° N.B.** i fori segnati servono per non crepare il plexiglass durante la piegatura    </td>
  
- successivamente ho creato e stampato, con filamento PLA, degli oggetti 3d per completare le parti mancanti:  
  - il [coperchio](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFiles/coperchio.stl) (che ospita il pulsante e il sensore di distanza a ultrasuoni)
  - il [piatto](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFiles/distributore.stl) che fa anche da distributore (al quale viene fissata tutta l'elettronica principale)
  - tre [passacavi](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFiles/passacavi.stl) (per tenere la struttura in plexiglass rigida e per non fare vedere i cavi passanti dal coperchio al distributore)
  - altri tipi di [viti](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFiles/VITI.stl) (che migliorano l'erogazione dei croccantini)

### prototipo
<table>
  <tr>
    <td>
      <img src="https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/1d8c1188-310b-4eeb-b495-11b66fe89119" alt="prototipo" width="200"/>
    </td>
    <td>
      <img src="https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/e6178876-4cb0-4c48-9613-3913fda39eaa" alt="circuito" width="500"/>
    </td>
  </tr>
</table>

________________________
### errori
- cavi troppo corti
- motorino troppo debole
- piatto distributore poco inclinato

### possibili miglioramenti futuri
in futuro si potrebbe migliorare il progetto, inserendo:
- un motorino dc che, vibrando, non faccia bloccare i croccantini nel contenitore
- un sensore per identificare quando c'è un gatto in avvicinamento, così da non far partire nessun attuatore e farlo mangiare senza distrazioni.
- algoritmo che proporzioni il cibo dato durante la giornata rispetto ad una quantità massima decisa dall'utente
- un collegamento al wifi per:
  - memorizzare in un giorno la quantità di cibo mangiato o altre informazioni
  - notificare all'utente informazioni, tra cui quanto ha mangiato il gatto o se sta per finire il cibo
- un display per gestire tutte le informazioni di setup iniziali e visualizzare gli stati in cui è il distributore durante l'arco delle giornate.

