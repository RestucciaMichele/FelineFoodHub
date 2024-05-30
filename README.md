![logoFelineFoodHub](https://github.com/RestucciaMichele/FelineFoodHub/assets/72453178/262522e9-989b-420c-ab3f-00e54a551863)



Progetto creato per il corso di [Sistemi Embedded](https://gitlab.di.unimi.it/sistemiembedded) dell'università Statale di Milano

### hardware
  - led rgb
  - bottone
  - sensore ultrasuoni
  - servomotore
  - cella di carico
### funzionamento
Il progetto prevede la realizzazione di un distributore automatico di croccantini che, durante l'arco della giornata, riempia la ciotola del cibo in modo autonomo. Il sistema utilizza una cella di carico per registrare il peso del cibo nella ciotola e attivare un servomotore quando necessario, garantendo così un rifornimento costante. Inoltre, un sensore a ultrasuoni controlla il livello di croccantini nel contenitore, segnalando quando la scorta sta per esaurirsi.


## miglioramenti
in futuro si potrebbe aumentare la complessità, inserendo:
- un display per gestire tutte le informazioni di setup iniziali e visualizzare gli stati in cui è il distributore durante l'arco delle giornate.
- un sensore per identificare quando c'è un gatto in avvicinamento, così da non far partire nessun attuatore e farlo mangiare senza distrazioni.
- un collegamento al wifi per:
  - memorizzare in un giorno la quantità di cibo mangiato o altre informazioni
  - notificare all'utente informazioni, tra cui quanto ha mangiato il gatto o se sta per finire il cibo
