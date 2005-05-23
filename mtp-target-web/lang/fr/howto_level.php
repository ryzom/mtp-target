<h2>How to create a level</h2>


<h3>Description</h3>
<ul>
<li>A level is made of one or more module and some global parameters</li>
<li>A module is a 3d object whith a collision shape and some local parameters.</li>
</ul>
<p>
The most simple way to make new level is tu reuse existing modules, and to place theme where you want.
</p>
<br/>
<br/>

<h3>Level</h3>

<p>A level is a text file using the <code>.level</code> extension, it is editable with any text editor like notepade.<br/>
this kind of file is made of blocs, each bloc is delimited by <b>[</b> and <b>]</b><br/>

<p>look at this simple example</p>
<pre>
[General]
LevelName = Where are you
Author = Skeet and Ace

[Module]
Pos = 0.0 0.0 1.0
Name = snow_target_50_flat

[StartPoint]
Pos = 0.3 21.4 9.2

[Camera]
Pos = 0 0 0

</pre>

There is 4 kind of blocs :
<ul>
<li><b>Module</b> : add a module to the level.</li>
<li><b>StartPoint</b> : add a player start point, from where the player will spawn at the begining of the session.</li>
<li><b>Camera</b> : set the camera position.</li>
<li><b>General</b>: add global parameter like gravity...</li>
</ul>
</p>
<br/>
<br/>

<h3>Module</h3>

<p>A bloc module must contains :</p>

<ul>
<li><b>pos</b> : x y z position of the module in 3d over X Y Z axes.</li>
<li><b>rot</b> : x y z w rotation of the module using axis angle (x y z = axis , w = angle)</li>
<li><b>scale</b> : x y z scale of the module in 3d over X Y Z axes.</li>
<li><b>name</b> module name (corresponding filename is "modulename<b>.module</b>").</li>
</ul>

<p>Here an example, to add the module <code>snow_score50</code> to the level, at position (x=0,y=0,z=0) without any rotation and unit size
</p>

<pre>
[Module]
pos = 0 0 0
rot = 0 1 0 0
scale = 1 1 1
name = snow_score50
</pre>
<br/>
<br/>

<h3>Bloc StartPoint</h3>

<p>Voici les param�tres qui doivent �tre mis dans un bloc StartPoint:</p>

<lu>
<li><b>pos</b> donne la position o� se d�marrera le joueur 0 en 3d sur les axes X Y Z.</li>
</lu>

<p>Il est possible de poser autant de bloc StartPoint que l'on veut, Il faut qu'il y ait au moins
autant de bloc StartPoint que de nombre joueur maximum dans le niveau (sinon il n'y en aura pas assez
pour placer tous les joueurs dans le niveau).</p>

<p>Par exemple, le bloc suivant permet d'ajouter un point de d�part pour un joueur � la position (0.09 1.5 9.6) dans le niveau:</p>

<pre>
[StartPoint]
pos = 0.09 1.5 9.6
</pre>

<h3>Bloc Camera</h3>

<p>Voici les param�tres qui doivent �tre mis dans un bloc Camera:</p>

<ul>
<li><b>pos</b> donne la position o� se situera la cam�ra en 3d sur les axes X Y Z.</li>
</ul>

<p>Par exemple, le bloc suivant place la cam�ra � la position (0.09 1.5 9.6) dans le niveau:</p>

<pre>
[Camera]
pos = 0.09 1.5 9.6
</pre>

<h3>Bloc General</h3>

<p>Voici les param�tres qui peuvent �tre mis dans un bloc General:</p>

<ul>
<li><b>levelName</b> est le nom du niveau qui sera affich� au d�marrage du niveau.</li>
<li><b>author</b> est le nom du cr�teur du niveau qui sera affich� au d�m�rrage du niveau.</li>
<li><b>maxOpenClose</b> est le nombre de fois que le joueur pourra appuyer sur la touche <code>Control</code> au maximum.</li>
<li><b>timeBeforeStart</b> est le temps en seconde au d�marrage avant de lancer les joueurs.</li>
<li><b>timeBeforeRestart</b> est le temps en seconde � la fin du niveau avant de passer au suivant.</li>
<li><b>timeBeforeCheck</b> est le temps en seconde avant de commencer � tester si le niveau est fini ou pas.</li>
<li><b>timeTimeout</b> est la dur�e maximum du niveau en seconde.</li>
<li><b>nbWaitingPlayers</b> est le nombre minimum de joueur necessaire pour commencer � jouer � ce niveau.</li>
<li><b>openAccelCoef</b> est un coefficient accelerateur appliqu� en mode vol.</li>
<li><b>closeAccelCoef</b> est un coefficient accelerateur appliqu� en mode roule.</li>
<li><b>sphereDensity</b> est la densit� du pingouin.</li>
<li><b>gravity</b> est la gravit� du niveau.</li>
<li><b>bounceWater</b> est la quantit� d'�nergie renvoi� en cas de contact avec l'eau (0=aucun rebond).</li>
<li><b>bounceVelWater</b> est la vitesse minimale pour que le rebond se fasse avec l'eau.</li>
<li><b>bounceClient</b> est la quantit� d'�nergie renvoi� en cas de contact avec un autre joueur (0=aucun rebond).</li>
<li><b>bounceVelClient</b> est la vitesse minimale pour que le rebond se fasse avec l'eau.</li>
<li><b>bounceScene</b> est la quantit� d'�nergie renvoi� en cas de contact avec les �lements du decors (0=aucun rebond).</li>
<li><b>bounceVelScene</b> est la vitesse minimale pour que le rebond se fasse avec l'eau.</li>
<li><b>minVelBeforeEnd</b> est la vitesse en dessous de laquelle le joueur est marqu� comme arret�.</li>
<li><b>nbMaxPlayers</b> est le nombre maximum de joueur que le niveau peut avoir.</li>
</ul>

<p>Il faut savoir que ces param�tres sont optionnels. Si vous ne mettez pas certaines valeurs, des valeurs
par d�faut d�fini dans le fichier de configuration du serveur seront utilis�s.</p>

<h2>Module</h2>

<p>Un module est un �lement graphique avec un �lement de collision ainsi que des param�tres d�crivant
le score, acc�leration, friction. Pour cr�er un module, vous pouvez soit reprendre un module d�j�
existant, le copier/coller pour en faire un nouveau avec juste des param�tres diff�rents mais le m�me
�lement graphique et collision. Ou alors vous pouvez faire un nouveau module, dans ce cas, il vous
faut maitriser 3ds max ainsi que le syst�me d'export de NeL.</p>

<p>Un module est d�crit dans un fichier texte �ditable avec notepad ou autre �diteur de texte classique
et a pour extension <code>.module</code>.</p>

<p>Le fichier contient des blocs qui commencent toujours par des crochets. Il y a plusieurs type de blocs:
<ul>
<li><b>TriColl</b> d�crit un syst�me de collision complexe pour le module.</li>
<li><b>Box</b> d�crit un syst�me de collision simple en boite pour le module.</li>
</ul>

<p>Voici les differents types de variables dans les blocs:</p>
<ul>
<li><b>pos</b> donne la position o� se trouvera la boite de colision en 3d sur les axes X Y Z.</li>
<li><b>rot</b> donne la rotation de la boite de colision en AxisAngle.</li>
<li><b>scale</b> donne le facteur de zoom la boite de colision sur les axes X Y Z.</li>
<li><b>score</b> est le score que le joueur aura s'il se pose sur ce module.</li>
<li><b>bounce</b> est le co�ficient de rebond sur ce module.</li>
<li><b>accel</b> est le co�ficient d'acc�leration du pingouin sur ce module.</li>
<li><b>friction</b> est le co�ficient de friction du pingouin sur ce module.</li>
</ul>

<h2>Glossaire</h2>

<h3>Rep�re</h3>
<p>Le jeu est en trois dimension, pour poser un �l�ment, il faut 3 coordon�es X Y Z. Pour ceux
qui connaissent on utilise le rep�re main droite, comme dans 3ds max. Pour les autres, cela veut dire
que l'axe X d�place l'�lement vers la droite si X est positif et vers la gauche si X est n�gatif.
Un Y positif fait avancer l'�lement vers l'avant et un Y n�gatif le fait aller en arri�re. Le Z est
l'�l�vation. L'eau est � Z=1, un Z plus grand met l'�l�ment plus haut.</p>

<h3>AxisAngle</h3>
<p>Ce syst�me permet de faire tourner un �lement sur les 3 axes de rotation. Tout d'abord on donne un axe
de rotation avec les 3 premi�res valeurs, et la 4eme valeur donne l'angle en radiant que l'�lement doit eff�ctuer
autour de l'axe. Par exemple un AxisAngle de <code>0 0 1 1.57</code> fera une rotation de l'�lement de 90 degr�e
autour de l'axe Z.</p>

<div id="footer"><hr><pre>Last updated: $Id$</pre></div>
