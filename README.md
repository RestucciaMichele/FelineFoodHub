

![logo](https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/097acf7a-c695-4cca-8fe9-1894ff191e88)

Progetto creato per il corso di [Sistemi Embedded](https://gitlab.di.unimi.it/sistemiembedded) dell'Università Statale di Milano

### hardware utilizzato
  - **attuatori**:
    - [led rgb](https://www.amazon.it/Youmile-emissione-Tri-Color-trasparenti-potrebbero/dp/B0897L6TZ8)
    - [servomotore](https://www.amazon.it/Aihasd-28YBJ-48-4-Phase-Stepper-Arduino/dp/B00U744GJU)
  - **sensori**:
    - [pulsante con leva](https://www.amazon.it/GOLRISEN-Interruttore-Finecorsa-Attuatore-Momentaneo/dp/B07F9QKTQQ)
    - [sensore ultrasuoni](https://www.amazon.it/CML-Modulo-Ultrasuoni-Distanza-Trasduttore-Misurazione/dp/B00JB1C142)
    - [cella di carico (1 kg)](https://www.amazon.it/elettronica-portatile-sensore-HX711-pesatura-Raspberry/dp/B076PYX5DW)
_______________________________
### funzionamento in breve
Il progetto prevede la realizzazione di un distributore di croccantini che, durante l'arco della giornata, riempia la ciotola del cibo in modo autonomo. Il sistema utilizza una cella di carico per registrare il peso del cibo presente nella ciotola e attivare un servomotore quando necessario, garantendo così un rifornimento costante. Inoltre, un sensore a ultrasuoni controlla il livello di croccantini nel contenitore, segnalando quando la riserva sta per esaurirsi.  
Il led RGB segnala diversi stati:
  - **in blu**, il contenitore non ha più abbastanza cibo da fornire
  - **in rosso**, il coperchio del contenitore è aperto per ricaricare la scorta di croccantini (in questa fase tutto il sistema è momentaneamente sospeso)
  - **in verde**, il servomotore sta fornendo cibo
_______________________________
### costruzione
- questo è lo schema del plexiglass (da 4 mm) da tagliare, con tutte le misure che ho usato per costruire la struttura.
  <table>
  <tr>
    <td>
      <img src="https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/c57f1120-a306-4538-b359-df634e556f45" alt="misure plexiglass" width="800"/>
    </td>
    <td style="padding-left: 20px;">
      N.B. le proporzioni nell'immagine non sono quellle reali. Serve solo per avere le indicazioni sulle misure    </td>
    </tr>
  </table>

- successivamente ho creato e stampato, con filamento PLA, degli oggetti 3d per completare le parti mancanti:  
  - il [coperchio](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFile/coperchio.stl) (che ospita il pulsante e il sensore ultrasuoni)
  - il [piatto](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFile/distributore.stl) che fa anche da distributore (al quale viene fissata tutta l'elettronica principale)
  - tre [passacavi](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFile/passacavi.stl) (per tenere la struttura in plexiglass rigida e per non fare vedere i cavi passanti dal coperchio al distributore)
  - altri tipi di [viti](https://github.com/RestucciaMichele/FelineFoodHub/blob/main/readmeFile/VITI.stl) (che migliorano l'erogazione dei croccantini)

________________________
### possibili miglioramenti futuri
in futuro si potrebbe aumentare la complessità, inserendo:
- un display per gestire tutte le informazioni di setup iniziali e visualizzare gli stati in cui è il distributore durante l'arco delle giornate.
- un sensore per identificare quando c'è un gatto in avvicinamento, così da non far partire nessun attuatore e farlo mangiare senza distrazioni.
- un collegamento al wifi per:
  - memorizzare in un giorno la quantità di cibo mangiato o altre informazioni
  - notificare all'utente informazioni, tra cui quanto ha mangiato il gatto o se sta per finire il cibo
