<center><h2>Comment changer de musique.</h2></center><br>
<br>
Vous aimiez bien la musique de Mtp Target cr�e par <a href="http://hulud.digital-murder.org/">Hulud</a>, mais maintenant (apres plusieur heures de jeu), vous voudriez bien �couter autre chose ?<br>
Je vais vous expliquer comment cr�er et utiliser une playlist de type m3u en utilisant winamp 3, si vous n'utilisez pas winamp essayez de <a href="http://www.google.com/search?q=m3u+create">chercher avec google : m3u create</a> pour votre lecteur mp3 pref�r�.<br>
D'abord selectionnez les fichiers mp3 et glissez les dans winamp. Votre fenetre "playlist" de winamp devrai �tre remplie avec vos mp3.<br>
Maintenant appuyez sur le bouton "save list" dans la fenetre "playlist" :<br> 
<?php echo "<img src=\"$image_dir/img/winamp_m3u_a.jpg\">"; ?><br>
<br>
Allez dans le dossier <b>client</b> de Mtp Target et sauvez sous <code>test.m3u</code><br>
<?php echo "<img src=\"$image_dir/img/winamp_m3u_b.jpg\">"; ?><br>
Votre m3u sauv�.<br>
<br>
Maintenant ajoutez la ligne suivante � votre mtp_target.cfg :<br>
<br>
<code>M3UList			= "test.m3u";</code><br>
<br>
Vous pouvez lancer Mtp Target et jouer en �coutant vos nouvelles musiques.<br> 

<br>
<br> 
