# Parkour
天天酷跑


Hallo zusammen, dies ist ein kleines Spiel, das dem Spiel "Temple Run" nachempfunden ist. "Temple Run" war vor etwa zehn Jahren in China sehr beliebt, und fast jeder, der ein Handy hatte, spielte dieses Spiel. Jetzt habe ich es auch einfach nachgebaut.

Ich habe es in C geschrieben und die SDL-Grafikbibliothek verwendet.

Wenn Sie die main-Methode ausführen, wird zuerst das Startmenü angezeigt. Zu diesem Zeitpunkt müssen Sie die Taste "1" drücken, um das Spiel zu starten.

<div align=center>
    <img src="https://github.com/myry07/Parkour/blob/main/resource/imgs/01.jpg" width="500" height="315">
</div>


Das ist das Spielbild. Der Hintergrund und die Gegner bewegen sich nach links, und sie wechseln ständig ihre Bilder, um den Effekt zu erzielen, dass die Figur nach rechts rennt.

<div align=center>
    <img src="https://github.com/myry07/Parkour/blob/main/resource/imgs/02.jpg" width="500" height="315">
</div>

Die Funktionen sind recht einfach, es gibt nur eine "w"-Taste zum Springen. Wenn Sie nicht rechtzeitig springen und mit einem Gegner kollidieren, wird die Gesundheitsleiste oben links reduziert. Wenn Sie einen Gegner überspringen, erhöht sich die Punktzahl oben rechts. Der Löwe ist am schnellsten, verursacht den höchsten Schaden und bringt natürlich die meisten Punkte.

<div align=center>
    <img src="https://github.com/myry07/Parkour/blob/main/resource/imgs/03.jpg" width="500" height="315">
</div>



Wenn Sie eine bestimmte Punktzahl erreichen, endet das Spiel. Ebenso endet es, wenn die Gesundheit auf null fällt.
