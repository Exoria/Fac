//
// General documentation and assesments

/**
   @mainpage
   @author Mathias Paulin <Mathias.Paulin@irit.fr>
  *
   @section intro_sec Introduction
   Cette documentation correspond à l'environnement de développement d'un logiciel d'infographie 3D utilisé
   pour les travaux pratiques de l'UE "Introduction à l'Informatique Graphique 3D" en Master 1 informatique
   de l'université Paul Sabatier, Toulouse.
  *
  \section install_sec Installation
  Télécharger l'archive <A HREF="http://www.irit.fr/~Mathias.Paulin/M1Info/IG3D/TP/minimalrenderer.tgz">minimalrenderer.tgz</a>
  et la décompresser dans un répertoire spécifique.
  Après décompression : les répertoires suivants seront générés :
  \li \c src : Répertoire contenant les source C++ du logiciel. Certains de ces sources devront être complétés et modifiés pour la réalisation
  des travaux pratiques
  \li \c shaders : Répertoire contenant les sources GLSL des shader qui seront utilisés et modifiés pendant les TPs
  \li \c data : Répertoire contenant des fichier 3D exemples
  \li \c doc : Répertoire contenant cette documentation
 *
  \subsection dependances Dépendances
 *
  Le code fourni, écrit en C++ n'a été testé que sous Linux et mac OS X et nécessite les environnement de développement
  suivant pour être exploité :
  @li Un compilateur C++
  @li L'environement de compilation cmake (version 2.6 au moins)
  @li L'environement de développement et d'éxécution OpenGL supportant OpenGL 3.2 - core. L'installation de l'environnement de développement
  et d'éxécution OpenGL 3.2 - core n'est pas décrite ici et nécessite la disponibilité d'une carte graphique récente (dite DirectX 10),
  l'instalation d'un driver adapté et des bibliothèques de développement associée. Se référer au site
  <A HREF="http://www.opengl.org/">opengl.org</a> pour toute documentation concernant openGL.
  @li L'environnement de développement QT 4.7 au moins avec, au moins, les modules QTCore, QTGui et QTOpenGL.
  @li Bien que non nécessaire pour la réalisation des travaux pratiques, nous invitons les étudiants à installer le logiciel de création 3D Blender
  pour créer et éditer les objets et scènes qui seront visualisées par le logiciel développé en TP.
 *
  \subsection compilation Compilation
  Le système de compilation utilisé par ce code est le système <A HREF="http://www.cmake.org/">cmake</a>.
  L'ensemble des sources de ce logiciel d'infographie 3D est composé de fichiers C++ définissant l'architecture de base et placés
  dans le répertoire \c src . Le répertoire \c glm contient une bibliothèque de gestion de vecteurs et de matrices en C++,
  développée pour être utilisée avec OpenGL puisque suivant les mêmes spécification que le langage GLSL. Voir le site
  <A HREF="http://glm.g-truc.net/">glm.g-truc.net</a> pour tout renseignements sur cette bibliothèque.
 *
  Le répertoire \c filelaoders contient le code source C++ nécessaire au chargement de fichiers de données pour le logiciel. Le code de
  ce répertoire ne sera pas édité ou modifié pour les TPs mais uniquement utilisé et étendu.
 *
  Le répertoire \c rendersystem contient le code C++ principal de l'aplication. Ce code C++ est composé d'une partie IHM dévelopée
  avec QT (qui ne fera pas l'objet de modification pour les TPs) et d'une partie \c renderer correspondant au code source C++ de
  l'application qui sera développée pendant les TPs. Seuls les fichiers renderer.h et renderer.cpp devront être modifiés pendant les TPs et
  remis à l'enseignant pour évaluation des travaux.
 *
  Pour compiler le programme, nous utiliserons la possibilité offerte par cmake de compiler en dehors de l'arborescence des sources.
  Les étudiants sont libres d'utiliser l'EDI qu'ils souhaitent (QTCreator, kdevelop, ...) mais aucune assistance ne sera apportée sur
  l'utilisation d'un EDI quelconque.
  Voici la marche à suivre pour compiler votre programme :
 *
  \subsubsection compilation_premiere Première compilation
  \li Créer (la première fois) un répertoire \c build dans le répertoire d'extraction de l'archive
  <A HREF="http://www.irit.fr/~Mathias.Paulin/M1Info/IG3D/TP/minimalrenderer.tgz">minimalrenderer.tgz</a>
  (au même niveau que les répertoires \c src, \c shaders, \c data et \c doc)
  \li Dans un terminal, se positionner dans ce répertoire et taper la commande de génération des makefiles : <b>cmake ../</b>
  \li Compiler le programme par la commande \b make

  \subsubsection compilation_toutes Toutes les autres compilations
  \li Compiler le programme par la commande \b make lancée dans le répertoire \c build

  \subsection execution Exécution
  \li Executer le programme en tappant la commande <b>./minimalrenderer</b> dans le répertoire \c bin
  (ou ../bin/minimalrenderer depuis le répertoire \c build)
 */

//-------------------------------------------

/**
   @page TP1 Scéance 1 : mise en place d'un environnement logiciel.

  L'objectif de cette séance et de comprendre et de mettre en place l'architecture générale d'une
  application de synthèse d'images 3D. Une telle application est généralement développée selon le motif de conception
  Modèle-Vue-Contrôleur

  @li Le modèle est la scène 3D et est représenté par des classes gérant les maillages, les textures, les caméra, ...
  @li La vue est implantée par un system de rendu construit au dessus des interfaces de programmation d'application de bas niveau (OpenGL ou
  DirectX).
  @li Le contrôleur est l'interface entre le système et la vue et gère les interactions avec l'utilisateur.

  Dans notre application, le modèle est écrit en C++, la vue en C++/OpenGL et le contrôleur utilise l'API Qt.
  
  Nous nous fondons sur la spécification <a href="http://www.opengl.org/registry/doc/glspec32.core.20091207.pdf">OpenGL 3.2 core</a> comme API de bas niveau. Il est recommandé de consulter cette spécification ainsi que le  <a href="http://www.opengl.org/sdk/docs/man3/">manuel de référence</a> de cette API pour bien comprendre les concepts manipulés.
 

@section managingGeometries 1 - Représentation de la géométrie pour un affichage par OpenGL.

Fichier renderer.cpp, classe rendersystem::MyGLMesh.

Les données géométriques affichées par notre application sont des maillages, définis dans le code source ou chargés depuis des fichiers externes,
qui doivent être préparés pour un affichage par OpenGL.

La première étape de cette séance de travaux pratiques concerne la programmation de la classe rendersystem::MyGLMesh,
qui est chargée de faire ces opérations. Cette classe est déclarée (et partielement vide) dans le fichier renderer.cpp.
rendersystem::MyGLMesh hérite de la classe Mesh et lui rajoute à la représentation des maillages les fonctionalités nécessaires pour un affichage
par OpenGL.

Pour pouvoir être affiché en OpenGL, un maillage doit être représenté sous la forme d'un <b>Vertex Array Object</b>, objet abstrait
OpenGL permettant de décrire l'organisation des données du maillage en mémoire et comment y accéder. Lorsque notre maillage est ainsi
représenté, son tracé peut alors être réalisé simplement en demandat à OpenGL d'utiliser l'objet et d'en dessiner les éléments.

Deux méthodes doivent être programmées pour implanter la classe rendersystem::MyGLMesh :
@li rendersystem::MyGLMesh::compileGL()

Le rôle de cette fonction est de créer les objets OpenGL et de décrire l'organisation des données dans ces objets pour en permettre le tracé.
Nous utilisons dans notre application les notions de VertexArrayObject et de VertexBufferObject pour représenter nos maillages. Les explications concernant l'utilisation
de ces objets vous seront données en scéance. La spécification de ces concepts est sur le site <A HREF="http://www.opengl.org/">opengl.org</a>.
Se reporter aux commentaires de la fonction dans le fichier renderer.cpp ou dans la documentation de la fonction pour avoir des indications sur la marche à suivre.

@li rendersystem::MyGLMesh::drawGL()

Le rôle de cette fonction est de donner les ordres OpenGL pour le dessin d'un maillage. 


<b>Avant de continuer</b>, vous devez ecrire le code de ces deux fonctions et compiler votre programme. Se reporter aux commentaires de la fonction dans le fichier renderer.cpp ou dans la documentation de la fonction pour avoir des indications sur la marche à suivre.


@section initOpenGL 2 - Initialisation générale de l'application.

Fichier renderer.cpp, fonction rendersystem::Renderer::initRessources()

Au lancement de l'application, les différents paramètres et états du pipeline de discrétisation OpenGL
doivent être définis et initialisés. Ces états vont paramétrer le pipeline ou le contrôleur
pour permettre à l'application de réaliser ses tâches. Dans notre cas, les paramètres et états à initialiser concernent
@li le mode de rendu OPENGL_H
@li les paramètres de caméra pour la visualisation de la scène

Toute application de synthèse d'images nécessite l'utilisation de resources externes pour fonctionner.
Lors de l'initialisation de l'application, les resources dont la durée de vie est celle de l'application doivent être initialisées.
Dans le cas de notre applicaiton exemple, développée dans le cadre de ces travaux pratiques, ces resources seront :
@li le maillage à visualiser
@li les shaders pour les étages \c Vertex et \c Fragment
@li les paramètres d'apparence de l'objet à visualiser (textures, materiau, ...)


@subsection generalSttings 2.1 - Initialisation des paramètres OpenGL.

Dans notre application, nous souhaitons visualiser nos modèles sous forme de faces pleines avec élimination des parties cachées.
Initialiser dans la fonction  rendersystem::Renderer::initRessources() les états OpenGL nécessaires.

@subsection viewSettings 2.2 - Initialisation des paramètres de vue.

La fonction rendersystem::Renderer::initView() permet de définir les paramètres par défaut pour la visualisation d'un maillage dans notre application.
Initialiser la matrice de vue rendersystem::Renderer::g_viewMatrix à partir des indications données en commentaire dans la fonction rendersystem::Renderer::initView().

@subsection loadingGeometry 2.3 - Chargement et compilation des données géométriques de l'application.

La fonction rendersystem::Renderer::initGeometry() est utilisée pour définir les objets qui seront visualisés. dans cette méthode, en utilisant la classe loaders::obj_mtl::ObjLoader de chargement de fichier Alias/Wavefront OBJ,
charger l'objet  "../data/camel.obj" et stockez ses différentes composantes dans un vecteur (std::vector<loaders::Mesh*>).
Transformez ces composantes en vecteur de primitives affichables en utilisant votre classe rendersystem::MyGLMesh.

@subsection loadingShaders Chargement et compilation des shaders.

La spécification OpenGL 3.2 core nécessite la définition de \c shaders pour la gestion des
différentes données transitant dans le pipeline de discrétisation. Dans notre application, les shaders
sont définis dans des fichiers sources glsl qui doivent être chargés, compilés et liés avant de pouvoir
être utilisés pour le rendu. L'objectif de la fonction rendersystem::Renderer::initShaders() est de préparer l'application
afin qu'elle utilise par défaut les shaders "../shaders/vertexdefault.glsl" et "../shaders/fragmentdefault.glsl".
Il est à noter qu'un shader doit être vue comme une unité de compilation qui est ensuite intégrée dans un programme.

Les opérations devant être faites pour pouvoir utiliser des shaders OpenGL dans notre application sont les suivantes :
<ol>
<li> Vertex shader :
    <ol>
        <li> Charger le source depuis le fichier "../shaders/vertexdefault.glsl"
        <li> Créer un objet OpenGL VERTEX_SHADER et y associer le source
        <li> Compiler le shader
        <li> Vérifier les erreurs de compilation
    </ol>
<li> Fragment shader :
    <ol>
        <li> Charger le source depuis le fichier "../shaders/fragmentdefault.glsl"
        <li> Créer un objet OpenGL FRAGMENT_SHADER et y associer le source
        <li> Compiler le shader
        <li> Vérifier les erreurs de compilation
    </ol>
<li> Programme :
    <ol>
        <li> Créer un programme OpenGL, stocker son id dans g_program et y associer les shaders
        <li> Fixer les positions des attributs géométriques en accord avec la classe rendersystem::MyGLMesh et les convention d'écriture
du source GLSL
        <ul>
            <li> inPosition --> index 0
            <li> inNormal --> index 1
            <li> inTexCoord --> index 2.
        </ul>
        <li> Lier le programme
        <li> Vérifier les erreurs d'édition de lien
    </ol>
</ol>

@section mainRenderfunction 3 - Fonction générale de rendu.

La fonction rendersystem::Renderer::render() est la fonction principale de calcul d'une image de synthèse. Appelée à chaque
rafraichissement de l'image son rôle est de séquencer l'ensemble des ordres de dessin de l'API de bas niveau.
Dans notre application simple, cette fonction à le rôle suivant.

<ol>
<li> Préparer de l'image pour le rendu :
    <ul>
        <li> Effacer les buffers de destination
    </ul>
<li> Construire les matrices de vue et de projection :
    <ul>
        <li> Définir une matrice de projection perspective
        <li> Définir la matrice de vue
        <li> Calculer la matrice de transformation des normales
        <li> Calculer la matrice MVP de passage du repère objet au repère image
    </ul>
<li> Activer et paramétrer les shader par defaut :
    <ul>
        <li> Activer le programme par défaut
        <li> Positionner les paramètres indépendants des objets : les matrices dans notre cas
    </ul>
<li> Dessiner les objets de la scène :
    <ul>
        <li> Dessiner tous les objets de la collection rendersystem::Renderer::g_meshes
    </ul>
</ol>


*/



//-------------------------------------------

/**
   @page TP2 Scéance 2 : navigation et exploration dans la scène.

   La navigation et l'exploration d'une sc!ène 3D pass par la gestion d'une métaphore abstraite de Caméra.
   Dans notre application, cette notion est simplement représentée par la matrice rendersystem::Renderer::g_viewMatrix, définissant la transformation permettant de passer du repère scène au repère camera.

   La navigation et l'exploration d'une scène correspond à la modification de cette matrice en fonction des interactions utilisateur. Ces
   interactions sont capturées par l'interface QT et transmises à la fonction rendersystem::Renderer::handleMouseEvent() que vous allez compléter afin de permettre une exploration
   simple d'un objet.

   Trois type de mouvements sont gérés par notre système initial :
   <li> Mouvement de type "Observe" (bouton gauche de la souris). Ce mouvement consiste à effectuer une rotation centrée de la scène autour des axes X et Y de la caméra
   <li> Mouvement de type "Pan"  (bouton droit de la souris). Ce mouvement consiste à effectuer une translation de la scène dans le plain (X, Y) de la caméra
   <li> Mouvement de type "Zoom" (bouton central de la souris). Ce mouvement consiste à réaliser une translation de la scène selon l'axe Z de la caméra

   La matrice rendersystem::Renderer::g_viewMatrix a été initialisée dans l'exercice @ref viewSettings et défini les proriétés de la caméra. Afin de pouvoir réaliser
   les transformation, il faut récupérer les axes X, Y et Z du repère de cette caméra. Les rotations et les translations seront réalisées en
   fonction des valeurs des variables @c dx et @c dy
   contenant le déplacement normalisé de la souris dans le repère de l'image.


*/

//-------------------------------------------

/**
   @page TP3 Scéance 3 : génération procédurale de maillage.

*/

//-------------------------------------------

/**
   @page DV1 Devoir 1 : construction d'une sphère géodésique.

   L'objectif de ce devoir est de programmer et d'ajouter à la liste des objets que l'on peut visualiser dans notre
application un solide Platonique, l'icosahèdre et d'utiliser ce solide comme élément de base pour la construction d'une sphère géodésique.

Dans ce devoir programmerez 2 classes, Icosahèdre et Geodesique permettant de construire un icosahedre selon la définition de ce solide accessible
<A HREF="http://fr.wikipedia.org/wiki/Icosa%C3%A8dre">ici</a>.
La sphère géodésique sera obtenue en divisant récursivement jusqu'à un niveau N les faces de l'icosahèdre et en projettant les sommets sur la sphère.


*/

//-------------------------------------------

/**
   @page TP4 Scéance 4 : calcul de l'éclairage.

*/

//-------------------------------------------

/**
   @page TP5 Scéance 5 : textures pour le contrôle de l'apparence.

*/

//-------------------------------------------

/**
   @page TP6 Scéance 6 : subdivision de lignes et extrusion.

*/

//-------------------------------------------

/**
   @page DV2 Devoir 2 : mettez en scène vos idées.

*/
