var NAVTREE =
[
  [ "M1 Informatique - IG3D - Minimal OpenGL Renderer", "index.html", [
    [ "Page principale", "index.html", null ],
    [ "Pages associées", "pages.html", [
      [ "Scéance 1 : mise en place d'un environnement logiciel.", "a00001.html", null ],
      [ "Scéance 2 : navigation et exploration dans la scène.", "a00002.html", null ],
      [ "Scéance 3 : génération procédurale de maillage.", "a00003.html", null ],
      [ "Devoir 1 : construction d'une sphère géodésique.", "a00004.html", null ],
      [ "Scéance 4 : calcul de l'éclairage.", "a00005.html", null ],
      [ "Scéance 5 : textures pour le contrôle de l'apparence.", "a00006.html", null ],
      [ "Scéance 6 : subdivision de lignes et extrusion.", "a00007.html", null ],
      [ "Devoir 2 : mettez en scène vos idées.", "a00008.html", null ],
      [ "Liste des bogues", "a00063.html", null ]
    ] ],
    [ "Modules", "modules.html", [
      [ "Data loading software components.", "a00057.html", [
        [ "Alias/Wavefront OBJ and MTL file format.", "a00058.html", null ],
        [ "Simple text file utilities.", "a00056.html", null ]
      ] ],
      [ "Qt-OpenGL graphical interface", "a00060.html", null ],
      [ "Simple OpenGL Rendering system", "a00062.html", [
        [ "RendererGlobalFunctions", "a00061.html", null ]
      ] ],
      [ "Utilities", "a00059.html", null ]
    ] ],
    [ "Liste des classes", "annotated.html", [
      [ "MainWindow", "a00009.html", null ],
      [ "OpenGLWidget", "a00010.html", null ],
      [ "Mesh", "a00011.html", null ],
      [ "Mesh::TriangleIndex", "a00012.html", null ],
      [ "Mesh::Vertex", "a00013.html", null ],
      [ "mtl_parser", "a00014.html", null ],
      [ "obj_parser", "a00015.html", null ],
      [ "ObjLoader", "a00016.html", null ],
      [ "ObjLoader::Face", "a00017.html", null ],
      [ "ObjLoader::Group", "a00018.html", null ],
      [ "ObjLoader::mtlMaterial", "a00019.html", null ],
      [ "ObjLoader::VertexObj", "a00020.html", null ],
      [ "ObjMesh", "a00021.html", null ],
      [ "SmoothGroup", "a00022.html", null ],
      [ "MouseEvent", "a00023.html", null ],
      [ "MyGLMesh", "a00024.html", null ],
      [ "Renderer", "a00025.html", null ],
      [ "Timer", "a00026.html", null ]
    ] ],
    [ "Index des classes", "classes.html", null ],
    [ "Hiérarchie des classes", "hierarchy.html", [
      [ "MainWindow", "a00009.html", null ],
      [ "OpenGLWidget", "a00010.html", null ],
      [ "Mesh", "a00011.html", [
        [ "SmoothGroup", "a00022.html", null ],
        [ "MyGLMesh", "a00024.html", null ]
      ] ],
      [ "Mesh::TriangleIndex", "a00012.html", null ],
      [ "Mesh::Vertex", "a00013.html", null ],
      [ "mtl_parser", "a00014.html", null ],
      [ "obj_parser", "a00015.html", null ],
      [ "ObjLoader", "a00016.html", null ],
      [ "ObjLoader::Face", "a00017.html", null ],
      [ "ObjLoader::Group", "a00018.html", null ],
      [ "ObjLoader::mtlMaterial", "a00019.html", null ],
      [ "ObjLoader::VertexObj", "a00020.html", null ],
      [ "ObjMesh", "a00021.html", null ],
      [ "MouseEvent", "a00023.html", null ],
      [ "Renderer", "a00025.html", null ],
      [ "Timer", "a00026.html", null ]
    ] ],
    [ "Membres de classe", "functions.html", null ],
    [ "Liste des espaces de nommage", "namespaces.html", [
      [ "gui", "a00050.html", null ],
      [ "loaders", "a00051.html", null ],
      [ "loaders::obj_mtl", "a00052.html", null ],
      [ "loaders::text", "a00053.html", null ],
      [ "rendersystem", "a00054.html", null ],
      [ "utils", "a00055.html", null ]
    ] ],
    [ "Membres de l'espace de nommage", "namespacemembers.html", null ],
    [ "Liste des fichiers", "files.html", [
      [ "core_profile_attributes.mm", "a00027.html", null ],
      [ "fileloader.cpp", "a00028.html", null ],
      [ "fileloader.h", "a00029.html", null ],
      [ "glassert.h", "a00030.html", null ],
      [ "labexplanations.h", "a00031.html", null ],
      [ "main.cpp", "a00032.html", null ],
      [ "mainwindow.cpp", "a00033.html", null ],
      [ "mainwindow.h", "a00034.html", null ],
      [ "mesh.cpp", "a00035.html", null ],
      [ "mesh.h", "a00036.html", null ],
      [ "objfileparser.cpp", "a00037.html", null ],
      [ "objfileparser.h", "a00038.html", null ],
      [ "objloader.cpp", "a00039.html", null ],
      [ "objloader.h", "a00040.html", null ],
      [ "objmesh.cpp", "a00041.html", null ],
      [ "objmesh.h", "a00042.html", null ],
      [ "opengl.h", "a00043.html", null ],
      [ "openglwidget.cpp", "a00044.html", null ],
      [ "openglwidget.h", "a00045.html", null ],
      [ "renderer.cpp", "a00046.html", null ],
      [ "renderer.h", "a00047.html", null ],
      [ "timer.h", "a00048.html", null ],
      [ "utils.h", "a00049.html", null ]
    ] ],
    [ "Répertoires", "dirs.html", [
      [ "src", "dir_cf9aac580884f5dffb8767cbe062ec24.html", [
        [ "fileloaders", "dir_4dad967bc9aa159b4ffc91a8a3542990.html", null ],
        [ "rendersystem", "dir_e7b4b217d06773c92739966e4c544c85.html", null ]
      ] ]
    ] ],
    [ "Membres de fichier", "globals.html", null ]
  ] ]
];

function createIndent(o,domNode,node,level)
{
  if (node.parentNode && node.parentNode.parentNode)
  {
    createIndent(o,domNode,node.parentNode,level+1);
  }
  var imgNode = document.createElement("img");
  if (level==0 && node.childrenData)
  {
    node.plus_img = imgNode;
    node.expandToggle = document.createElement("a");
    node.expandToggle.href = "javascript:void(0)";
    node.expandToggle.onclick = function() 
    {
      if (node.expanded) 
      {
        $(node.getChildrenUL()).slideUp("fast");
        if (node.isLast)
        {
          node.plus_img.src = node.relpath+"ftv2plastnode.png";
        }
        else
        {
          node.plus_img.src = node.relpath+"ftv2pnode.png";
        }
        node.expanded = false;
      } 
      else 
      {
        expandNode(o, node, false);
      }
    }
    node.expandToggle.appendChild(imgNode);
    domNode.appendChild(node.expandToggle);
  }
  else
  {
    domNode.appendChild(imgNode);
  }
  if (level==0)
  {
    if (node.isLast)
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2plastnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2lastnode.png";
        domNode.appendChild(imgNode);
      }
    }
    else
    {
      if (node.childrenData)
      {
        imgNode.src = node.relpath+"ftv2pnode.png";
      }
      else
      {
        imgNode.src = node.relpath+"ftv2node.png";
        domNode.appendChild(imgNode);
      }
    }
  }
  else
  {
    if (node.isLast)
    {
      imgNode.src = node.relpath+"ftv2blank.png";
    }
    else
    {
      imgNode.src = node.relpath+"ftv2vertline.png";
    }
  }
  imgNode.border = "0";
}

function newNode(o, po, text, link, childrenData, lastNode)
{
  var node = new Object();
  node.children = Array();
  node.childrenData = childrenData;
  node.depth = po.depth + 1;
  node.relpath = po.relpath;
  node.isLast = lastNode;

  node.li = document.createElement("li");
  po.getChildrenUL().appendChild(node.li);
  node.parentNode = po;

  node.itemDiv = document.createElement("div");
  node.itemDiv.className = "item";

  node.labelSpan = document.createElement("span");
  node.labelSpan.className = "label";

  createIndent(o,node.itemDiv,node,0);
  node.itemDiv.appendChild(node.labelSpan);
  node.li.appendChild(node.itemDiv);

  var a = document.createElement("a");
  node.labelSpan.appendChild(a);
  node.label = document.createTextNode(text);
  a.appendChild(node.label);
  if (link) 
  {
    a.href = node.relpath+link;
  } 
  else 
  {
    if (childrenData != null) 
    {
      a.className = "nolink";
      a.href = "javascript:void(0)";
      a.onclick = node.expandToggle.onclick;
      node.expanded = false;
    }
  }

  node.childrenUL = null;
  node.getChildrenUL = function() 
  {
    if (!node.childrenUL) 
    {
      node.childrenUL = document.createElement("ul");
      node.childrenUL.className = "children_ul";
      node.childrenUL.style.display = "none";
      node.li.appendChild(node.childrenUL);
    }
    return node.childrenUL;
  };

  return node;
}

function showRoot()
{
  var headerHeight = $("#top").height();
  var footerHeight = $("#nav-path").height();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  navtree.scrollTo('#selected',0,{offset:-windowHeight/2});
}

function expandNode(o, node, imm)
{
  if (node.childrenData && !node.expanded) 
  {
    if (!node.childrenVisited) 
    {
      getNode(o, node);
    }
    if (imm)
    {
      $(node.getChildrenUL()).show();
    } 
    else 
    {
      $(node.getChildrenUL()).slideDown("fast",showRoot);
    }
    if (node.isLast)
    {
      node.plus_img.src = node.relpath+"ftv2mlastnode.png";
    }
    else
    {
      node.plus_img.src = node.relpath+"ftv2mnode.png";
    }
    node.expanded = true;
  }
}

function getNode(o, po)
{
  po.childrenVisited = true;
  var l = po.childrenData.length-1;
  for (var i in po.childrenData) 
  {
    var nodeData = po.childrenData[i];
    po.children[i] = newNode(o, po, nodeData[0], nodeData[1], nodeData[2],
        i==l);
  }
}

function findNavTreePage(url, data)
{
  var nodes = data;
  var result = null;
  for (var i in nodes) 
  {
    var d = nodes[i];
    if (d[1] == url) 
    {
      return new Array(i);
    }
    else if (d[2] != null) // array of children
    {
      result = findNavTreePage(url, d[2]);
      if (result != null) 
      {
        return (new Array(i).concat(result));
      }
    }
  }
  return null;
}

function initNavTree(toroot,relpath)
{
  var o = new Object();
  o.toroot = toroot;
  o.node = new Object();
  o.node.li = document.getElementById("nav-tree-contents");
  o.node.childrenData = NAVTREE;
  o.node.children = new Array();
  o.node.childrenUL = document.createElement("ul");
  o.node.getChildrenUL = function() { return o.node.childrenUL; };
  o.node.li.appendChild(o.node.childrenUL);
  o.node.depth = 0;
  o.node.relpath = relpath;

  getNode(o, o.node);

  o.breadcrumbs = findNavTreePage(toroot, NAVTREE);
  if (o.breadcrumbs == null)
  {
    o.breadcrumbs = findNavTreePage("index.html",NAVTREE);
  }
  if (o.breadcrumbs != null && o.breadcrumbs.length>0)
  {
    var p = o.node;
    for (var i in o.breadcrumbs) 
    {
      var j = o.breadcrumbs[i];
      p = p.children[j];
      expandNode(o,p,true);
    }
    p.itemDiv.className = p.itemDiv.className + " selected";
    p.itemDiv.id = "selected";
    $(window).load(showRoot);
  }
}

