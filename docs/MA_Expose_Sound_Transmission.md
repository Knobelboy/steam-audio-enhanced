## Exposé zur Masterarbeit

![TH Logo](TH%20Logo.pdf)

Technische Hochschule Köln  
Fakultät für Informations-, Medien- und Elektrotechnik  

---

### Exposé zur Masterarbeit

**Real-time, Physically-Based Sound Transmission  
for Immersive Virtual Environments**

TH Köln  
Campus Deutz  
Medientechnologie Master

- **vorgelegt von**: Marvin Winkler (11118106)  
- **Betreuer**: Prof. Dr. Christoph Pörschmann; Prof. Dr. Ulrich Reiter

Köln, 30. Juli 2025

---

### Motivation

Sound ist eine kritische Komponente immersiver virtueller Umgebungen. Während moderne Game-Audio-Engines bei der Modellierung von Schallausbreitungsphänomenen wie Reflexionen, Beugung und Nachhall hervorragend abschneiden, fehlt der Simulation der Schalltransmission durch feste Objekte oft die physikalische Genauigkeit. Engines wie Steam Audio modellieren die Transmission typischerweise als vereinfachten, mehrbandigen Entzerrungsfilter (EQ), der auf dem Material eines Hindernisses basiert. Dieser Ansatz ist zwar rechnerisch effizient, berücksichtigt aber nicht die komplexe, winkelabhängige Natur der Schalldämmung, wie sie in der Bauakustik verstanden wird.

Diese Vereinfachung führt zu einer Wahrnehmungsdiskrepanz. Die Art und Weise, wie eine Ziegelwand ein Gespräch dämpft oder ein Glasfenster Straßenlärm abschwächt, wird nicht mit hoher Genauigkeit erfasst. Für Anwendungen, die hohen Realismus erfordern – wie Architekturvisualisierung, akustische Simulation und Serious Games für Training – ist diese Einschränkung erheblich.

Das aktuelle akademische Paper „Interactive real-time auralization of airborne sound insulation in buildings" von Imran et al. (2021) präsentiert ein detailliertes Modell zur Berechnung der Schalltransmission basierend auf etablierten physikalischen Prinzipien und ISO-Standards. Es berücksichtigt Faktoren wie den Einfallswinkel, Materialeigenschaften und die Unterscheidung zwischen resonanter und erzwungener Transmission. Diese Forschung bietet eine klare mathematische Grundlage für eine realistischere Simulation.

Diese Arbeit zielt darauf ab, die Lücke zwischen akademischer Forschung in der Bauakustik und der praktischen Anwendung in interaktiven 3D-Umgebungen zu schließen. Das zentrale Ziel ist es, ein hochpräzises, physikalisch basiertes Schalltransmissionsmodell, basierend auf der Arbeit von Imran et al., in einer Echtzeit-Audio-Engine zu implementieren, um ein glaubwürdigeres und immersiveres Hörerlebnis zu schaffen.

### Stand der Technik

- **Steam Audio**: Ein hochmodernes, Open-Source-SDK für Audioausbreitung in interaktiven Umgebungen. Es bietet robuste Echtzeitsimulation von Reflexionen, Beugung und Pfadfindung. Sein Transmissionsmodell ist effizient, aber vereinfacht, indem es einen 3-Band-EQ basierend auf Materialeigenschaften verwendet, ohne den Einfallswinkel zu berücksichtigen.
- **Imran et al. (2021) Modell**: Ein spezialisiertes Auralisierungsmodell für die Bauakustik. Seine Hauptstärke ist die detaillierte, winkelabhängige Berechnung von Transmissionskoeffizienten \( \tau(\theta) \) für monolithische Elemente wie Wände und Fenster. Es ist gegen ISO 12354 validiert und nachweislich leistungsfähig genug für Echtzeitinteraktion. Es handelt sich jedoch um einen Forschungsprototyp, dem die umfassenden Funktionen einer vollständigen Audio-Engine fehlen (z.B. Beugung, komplexe Pfadfindung, produktionsreife Integrationen).
- **Neural Acoustic Fields (Luo et al., 2022)**: Jüngste Arbeiten vom MIT führen implizite neuronale Repräsentationen für die akustische Ausbreitung ein, die die Speicheranforderungen drastisch reduzieren und gleichzeitig eine hohe Genauigkeit beibehalten. Obwohl vielversprechend für vorberechnete Szenarien, erfordert der Ansatz derzeit Offline-Training und ist nicht für dynamische Echtzeit-Materialänderungen geeignet.
- **Frequenzabhängige TD-DG-Methoden (Wang & Hornikx, 2020)**: Fortgeschrittene zeitbereichsbasierte diskontinuierliche Galerkin-Methoden ermöglichen eine genaue Simulation der frequenzabhängigen Schalltransmission. Diese in Applied Acoustics veröffentlichte Arbeit bietet ausgefeilte numerische Techniken zur Modellierung von Impedanzdiskontinuitäten, obwohl die Rechenanforderungen für Echtzeit-Spielanwendungen herausfordernd bleiben.
- **Project Acoustics 3.0 (Microsoft, 2024)**: Zeigt Branchenfortschritte mit 2x schnellerem Baking und 3x Reduzierung der pro-Quellen-Berechnung unter Verwendung wellenbasierter Simulation. Obwohl es hochwertige Akustik bietet, unterscheidet sich sein Fokus auf vorberechnete Lösungen von unserem Ziel der dynamischen, physikalisch basierten Transmissionsmodellierung.

### Ziele und Forschungsfragen der Arbeit

Das primäre Ziel dieser Arbeit ist die Entwicklung und Evaluierung einer physikalisch plausiblen Echtzeit-Schalltransmissionssimulation innerhalb der Unity-Game-Engine.

Dies führt zu folgenden Forschungsfragen:

1. **Wie können die winkelabhängigen Transmissionsberechnungen aus dem Paper von Imran et al. in die Steam Audio-Pipeline integriert werden?**
2. **Welche zusätzliche Rechenlast entsteht durch das neue Transmissionsmodell im Vergleich zur Standard-Implementierung von Steam Audio?**
3. **Inwieweit verbessert das neue Modell die physikalische Genauigkeit und den wahrgenommenen Realismus der Schalltransmission in einer virtuellen Umgebung?**

Um diese zu beantworten, werden folgende konkrete Ziele gesetzt:

- **Ziel 1: Implementierung**: Integration der mathematischen Kernformeln für winkelabhängige Transmission aus dem Paper in ein Echtzeit-Audio-Framework.
- **Ziel 2: Leistungsanalyse**: Profilierung der Implementierung zur Messung der CPU-Auswirkungen und Sicherstellung, dass sie Echtzeitanforderungen erfüllt (d.h. Latenz unter 50 ms).
- **Ziel 3: Validierung**:
  - **Akustische Validierung**: Nachstellung der Validierungsszenarien aus dem Paper, um die Ausgabe des implementierten Modells mit den veröffentlichten Ergebnissen und ISO-Standards zu vergleichen.
  - **Perzeptuelle Evaluation**: Hörexperten werden gebeten, den wahrgenommenen Realismus des neuen Modells subjektiv mit der Baseline-Steam Audio-Implementierung und realen Aufnahmen zu vergleichen.

**Umfang**: Die Arbeit wird sich auf monolithische, planare Oberflächen (Wände, Fenster) konzentrieren. Nicht-planare Oberflächen (gekrümmte Wände, Kuppeln) sind explizit außerhalb des Umfangs. Als optionale Erweiterung können zusammengesetzte Strukturen (z.B. ein Fenster in einer Wand) untersucht werden, falls die Zeit es erlaubt.

### Vorgeschlagene Methodik

#### Mathematische Grundlage

Die Implementierung wird auf den Kerngleichungen basieren, die von Imran et al. (2021) präsentiert wurden, mit Fokus auf:

- **Winkelabhängige Transmission \( \tau(\theta) \)**: Die primäre Berechnung wird die Formeln für den Transmissionskoeffizienten verwenden, der vom Einfallswinkel \( \theta \), der Frequenz und den Materialeigenschaften (Masse, Dämpfung usw.) abhängt.
- **Unterscheidung der kritischen Frequenz**: Die Implementierung wird korrekt zwischen Berechnungen oberhalb und unterhalb der kritischen Frequenz des Materials unterscheiden und die entsprechenden Formeln für erzwungene und resonante Transmission anwenden, wie im Paper beschrieben (z.B. Gleichungen 17, 18, 22).
- **Abstrahlungseffizienz für endliche Platten**: Für spezifische Elemente wie Fenster wird das Modell die Abstrahlungseffizienz \( \sigma(\theta) \) für endlich große Platten einbeziehen (Gleichung 21).

#### Simulation der Flächenquellen-Transmission

Ein Schlüsselaspekt der realistischen Schalltransmission durch Objekte wie Wände und Fenster ist, dass die gesamte Oberfläche vibriert und Schall abstrahlt, anstatt dass Schall einfach durch einen einzigen Punkt hindurchgeht. Dies erzeugt einen „Flächenquellen"-Effekt, der für den perzeptuellen Realismus entscheidend ist, insbesondere in Kombination mit HRTFs. Das Paper von Imran et al. adressiert dies durch:

1. **Wandsegmentierung („Patches")**: Aufteilung großer Oberflächen in kleinere Segmente oder „Patches".
2. **Sekundärquellenstrahlung**: Behandlung jedes dieser Patches auf der Empfangsseite als neue, unabhängige Schallquelle („Sekundärquelle").
3. **Summierung im Empfangsraum**: Das endgültige Schallfeld ist die akustische Summe des von allen diesen Sekundärquellen abgestrahlten Schalls.

Um dies in Echtzeit effizient zu simulieren, wird eine **Dynamische Sekundärquellen-Spawning**-Strategie angewendet:

- **Oberfläche abtasten**: Wenn der Raytracer von Steam Audio eine transmissive Oberfläche identifiziert, fungiert dieser Treffer als Sonde.
- **Sekundärquellen generieren**: Auf der Empfangsseite der Wand wird dynamisch ein kleines Gitter neuer, virtueller Schallquellen erstellt (z.B. ein 3x3- oder 5x5-Gitter). Die Anzahl der Quellen kann ein Leistungsregler sein, möglicherweise an eine Qualitätseinstellung oder Entfernung gebunden.
- **Übertragene Energie verteilen**: Die gesamte durch die Wand übertragene Schallenergie (berechnet mit den physikalisch basierten Formeln des Papers) wird auf diese Sekundärquellen verteilt. Um die ungleichmäßige Vibration einer Ebene zu simulieren (z.B. mehr Bewegung in der Mitte als an festen Kanten), wird eine Gewichtungsfunktion (z.B. ein 2D-Gauß- oder kosinusbasierter Abfall) angewendet, die zentralen Quellen mehr Energie gibt.
- **Mit vorhandener Engine rendern**: Diese gewichteten Sekundärquellen werden dann in die Steam Audio-Engine eingespeist, die ihr Rendering einschließlich HRTF-Anwendung übernimmt.

Eine Visualisierung wird erstellt, um den Unterschied zwischen dem aktuellen Punktquellen-Transmissionsmodell (bei dem Schall durch einen einzigen Punkt hindurchgeht) und dem vorgeschlagenen Flächenquellen-Transmissionsmodell (bei dem die gesamte Oberfläche Schall abstrahlt) deutlich zu demonstrieren. Dieser Vergleich wird helfen, die perzeptuellen Verbesserungen des neuen Ansatzes zu veranschaulichen.

#### Codebase & Implementierungsstrategie

Die Hauptherausforderung besteht darin, diese detaillierten Berechnungen und die Flächenquellen-Simulation in ein bestehendes, komplexes System zu integrieren. Mehrere Strategien wurden als potenzielle Ausgangspunkte für das Projekt in Betracht gezogen:

**Option A: Forken von Steam Audio**

Dies ist der vielversprechendste und integrierteste Ansatz zur Erreichung der Arbeitsziele. Durch die Modifikation der C++-Kernbibliothek von Steam Audio können wir das bestehende Transmissionsmodell ergänzen.

- **Begründung**: Diese Option ermöglicht eine nahtlose Integration mit den anderen robusten Funktionen von Steam Audio (Reflexionen, Beugung, Pfadfindung). Der vorhandene Raytracer liefert bereits die notwendigen geometrischen Daten (Trefferpunkte, Normalen), und das Framework zur Anwendung von Audio-DSP-Effekten ist bereits vorhanden. Dies vermeidet redundante Berechnungen und gewährleistet akustische Konsistenz innerhalb einer einheitlichen Klanglandschaft, was für immersive virtuelle Umgebungen entscheidend ist.
- **Ausführungsplan**:
  1. **Repository forken**: Erstellen eines Forks des offiziellen Steam Audio-Repositorys.
  2. **Kernlogik identifizieren**: Analyse des `core/src/core`-Verzeichnisses. Die Schlüsseldateien für Modifikationen sind wahrscheinlich `direct_simulator.cpp` (wo Okklusions-/Transmissionsstrahlen verfolgt und Materialeigenschaften gesammelt werden) und `direct_effect.cpp` (wo das Transmissions-DSP angewendet wird).
  3. **Materialeigenschaften erweitern**: Die bestehende `IPLMaterial`-Struktur muss möglicherweise erweitert werden, um zusätzliche physikalische Eigenschaften einzuschließen, die das neue Modell benötigt, wie Steifigkeit (Elastizitätsmodul) und interner Dämpfungsfaktor.
  4. **Berechnung ersetzen**: Ergänzung des aktuellen 3-Band-EQ-Lookups um eine neue Funktion, die \( \tau(\theta) \) basierend auf den Gleichungen des Papers berechnet. Der Einfallswinkel \( \theta \) kann aus der Strahlrichtung und der vom Raytracer bereitgestellten Oberflächennormalen berechnet werden.
  5. **Sekundärquellen-Spawning implementieren**: Integration der dynamischen Sekundärquellen-Spawning-Logik, Verteilung der berechneten übertragenen Energie auf die erzeugten Quellen.
  6. **Als Filter anwenden**: Der berechnete Transmissionskoeffizient (ein frequenzabhängiger Wert) wird verwendet, um einen benutzerdefinierten Filter zu generieren, der auf den direkten Schallpfad in `direct_effect.cpp` angewendet wird.

**Option B: Binaural Rendering Toolbox (BRT)**

BRT ist eine C++-Bibliothek, die hauptsächlich für hochwertiges binaurales Audio-Rendering in psychoakustischen Experimenten entwickelt wurde.

- **Begründung**: BRT zeichnet sich durch präzises binaurales Rendering aus, was eine Kernanforderung der Arbeit ist. Sein Fokus auf Psychoakustik steht im Einklang mit dem Ziel des wahrgenommenen Realismus.
- **Bewertung**: Obwohl BRT exzellente Binauralisierung bietet, ist es keine vollwertige Game-Audio-Engine mit integrierten Schallausbreitungsfunktionen wie Reflexionen, Beugung oder komplexe Pfadfindung. Die Verwendung von BRT würde bedeuten, die gesamte Schallausbreitungs-Pipeline (einschließlich des Transmissionsmodells) von Grund auf zu erstellen oder sie mit einer anderen Engine zu integrieren. Dies würde wahrscheinlich zu einem fragmentierten akustischen Modell führen, bei dem verschiedene Phänomene von unterschiedlichen Systemen behandelt werden, was es schwierig macht, eine kohärente und einheitliche Klanglandschaft zu erreichen.

**Option C: Eigenständiges Unity-Plugin**

- **Beschreibung**: Dieser Ansatz beinhaltet die Erstellung eines separaten C#-Skripts oder nativen Plugins innerhalb von Unity, das unabhängig von Steam Audio läuft. Es würde eigene Raycasts durchführen und eigene Audiofilterung anwenden.
- **Bewertung**: Diese Option ist im Allgemeinen weniger wünschenswert. Sie würde zu redundantem Raycasting führen (eines von Steam Audio, eines von unserem Plugin), potenziellen Synchronisationsproblemen und einem akustischen Modell, das nicht mit den bestehenden Ausbreitungsfunktionen von Steam Audio vereinheitlicht ist. Diese „aufgesetzte" Lösung wäre ineffizient und weniger robust als eine direkte Modifikation einer Kern-Engine.

**Option D: Andere Game-Audio-Engines/Bibliotheken**

Es existieren mehrere andere Game-Audio-Engines und -Bibliotheken, einige mit räumlichen Audiofähigkeiten:

- **Wwise / FMOD**: Branchenübliche kommerzielle Audio-Middleware-Lösungen. Obwohl leistungsstark und funktionsreich, würde ihre Closed-Source-Natur oder komplexe Lizenzmodelle es schwierig machen, ihre Kern-Schallausbreitungsalgorithmen zu modifizieren, um das detaillierte Transmissionsmodell aus dem Paper zu implementieren.
- **SoLoud / OpenAL Soft**: Open-Source-Audiobibliotheken, die grundlegendes 3D-Positionsaudio bieten. Ihnen fehlen jedoch erweiterte Schallausbreitungsfunktionen wie geometrische Akustik (Reflexionen, Beugung, komplexe Okklusion) und würden den Aufbau der gesamten Ausbreitungs-Pipeline, einschließlich des Transmissionsmodells, von Grund auf erfordern. Dies wäre ein erheblich größeres Unterfangen als die Erweiterung von Steam Audio.

**Option E: Spezialisierte akustische Simulations-Frameworks**

Frameworks wie EVERTims oder ITA Simulation Scheduler sind für Echtzeit-Auralisation in der Architekturakustik konzipiert.

- **Bewertung**: Diese Frameworks sind konzeptionell näher an der Domäne des Papers. Sie sind jedoch typischerweise nicht als integrierte Game-Engines konzipiert und erfordern möglicherweise erheblichen Aufwand zur Anpassung für Echtzeitinteraktion innerhalb von Unity, einschließlich der Handhabung spielspezifischer Funktionen, Rendering und Physikintegration. Obwohl wertvoll für die Forschung, sind sie möglicherweise nicht der effizienteste Ausgangspunkt für eine Echtzeit-Game-Audio-Lösung.

Angesichts des Projektziels, eine realistische Wand-/Fenster-Audiotransmissionssimulation zu erstellen, die in Echtzeit in Unity läuft und gut mit anderen Ausbreitungseffekten integriert ist, **bleibt das Forken von Steam Audio (Option A) die praktikabelste und bevorzugte Strategie.**

#### Evaluierungsplan

1. **Leistung**: Eine Unity-Testszene wird erstellt mit einer Quelle und einem Hörer, getrennt durch verschiedene Materialien (Ziegelwand, Glasfenster). Die CPU-Auslastung der direkten Schallsimulation wird profiliert und zwischen der geforderten Implementierung und dem originalen Steam Audio verglichen.
2. **Validierung**: Die \( D_{nT} \) (Standardisierte Pegeldifferenz)-Werte werden aus der Simulationsausgabe berechnet und gegen die Diagramme im Paper von Imran et al. (z.B. Abbildungen 7, 10, 11) aufgetragen, um die physikalische Genauigkeit der Implementierung zu verifizieren.
3. **Wahrnehmung**: Ein kleiner Hörtest wird durchgeführt. Experten werden A/B-Vergleiche verschiedener Szenarien präsentiert (z.B. Sprache hinter einer Wand, Verkehr außerhalb eines Fensters) mit sowohl dem Original- als auch dem neuen Modell und gebeten, den wahrgenommenen Realismus zu bewerten.

### Zeitplan der Arbeit

1. **Einleitung (2 Wochen)**: Motivation, Forschungsfragen, Literaturübersicht.
2. **Analyse & Design (3 Wochen)**: Tiefgehende Analyse der Steam Audio-Codebasis und des mathematischen Modells. Design der neuen Softwarekomponenten und Datenstrukturen.
3. **Implementierung (8 Wochen)**: Forken des Repositorys, Implementierung der neuen Transmissionsberechnungen und Materialeigenschaften im C++-Kern und Bereitstellung notwendiger Parameter für den Unity-Editor.
4. **Evaluierung (3 Wochen)**: Durchführung von Leistungsprofilierung, akustischen Validierungstests und der perzeptuellen Evaluierung.
5. **Dokumentation (8 Wochen)**: Dokumentation der Forschung, Methodik, Ergebnisse und Schlussfolgerung. Diskussion zukünftiger Arbeiten einschließlich zusammengesetzter Strukturen (Fenster in Wänden) und anderer fortgeschrittener Transmissionsszenarien.

---

### Literaturverzeichnis

- [1] Imran, M., Heimes, A., & Vorländer, M. (2021). Interactive real-time auralization of airborne sound insulation in buildings. _Acta Acustica_, 5, 19. DOI: `https://doi.org/10.1051/aacus/2021013`
- [2] ISO 12354-1:2017. Building acoustics – Estimation of acoustic performance of buildings from the performance of elements – Part 1: Airborne sound insulation between rooms. International Organization for Standardization, Geneva, Switzerland.
- [3] Luo, A., Du, Y., Tarr, M., Tenenbaum, J., Torralba, A., & Gan, C. (2022). Learning Neural Acoustic Fields. In: _Advances in Neural Information Processing Systems_, 35, 3165-3177. NeurIPS 2022.
- [4] Microsoft (2022). Project Acoustics 3.0: Next-generation wave acoustics for games. Microsoft Developer Blog. `https://developer.microsoft.com/en-us/games/articles/2022/08/project-acoustics-30-is-now-available/`
- [5] Valve Corporation (2024). Steam Audio SDK Documentation. Version 4.6.0. `https://valvesoftware.github.io/steam-audio/`  
  GitHub Repository: `https://github.com/ValveSoftware/steam-audio`
- [6] Wang, H., Yang, J., & Hornikx, M. (2020). Frequency-dependent transmission boundary condition in the acoustic time-domain nodal discontinuous Galerkin model. _Applied Acoustics_, 164, 107280. DOI: `https://doi.org/10.1016/j.apacoust.2020.107280`

#### Software und Tools

- Unity Technologies (2024). Unity Real-Time Development Platform. Version 2024.3 LTS. `https://unity.com/`
- Audiokinetic Inc. (2024). Wwise - Interactive Audio Engine. Version 2023.1+. `https://www.audiokinetic.com/`
- Firelight Technologies (2024). FMOD Studio - Adaptive Audio Tools. Version 2.0+. `https://www.fmod.com/`
- González-Toledo, D., Molina-Tanco, L., Cuevas-Rodríguez, M., Majdak, P., & Reyes-Lecuona, A. (2023). The Binaural Rendering Toolbox. A Virtual Laboratory for Reproducible Research in Psychoacoustics. In: _Proceedings of the 10th Convention of the European Acoustics Association_, Forum Acusticum 2023, Turin, Italy. GitHub: `https://github.com/GrupoDiana/BRTLibrary`
- OpenAL Soft Contributors (2024). OpenAL Soft - Cross-platform 3D audio API. `https://openal-soft.org/`
- SoLoud Contributors (2024). SoLoud - Easy to use audio engine. `https://github.com/jarikomppa/soloud`
- Poirier-Quinot, D., Katz, B. F. G., & Noisternig, M. (2017). EVERTims: Open source framework for real-time auralization in architectural acoustics and virtual reality. In: _Proceedings of the 20th International Conference on Digital Audio Effects (DAFx-17)_, Edinburgh, UK, September 5-9, 2017. `http://evertims.ircam.fr`
- Institute of Technical Acoustics, RWTH Aachen University (2024). ITA Simulation Scheduler. `https://www.akustik.rwth-aachen.de/`

---

- Konvertiert das LaTeX-Dokument in gut strukturiertes Markdown mit Überschriften, Listen, Links und Inline-Mathe.