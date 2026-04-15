// demo.js

Module.onRuntimeInitialized = () => { start(); }

const EMPTY = 0;    // empty square 
const TREE = 1;     // tree square 
const TENT = 2;     // tent square 
const GRASS = 3;    // grass square 
const BIGTREE = 4;  // big tree square 

let _square2str = [" ", "x", "*", "-", "#"];

var g = null;
var taille_case = 0;

function square2str(s) {
    return _square2str[s];
}

function printGame(g) {
    var text = "";
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var s = Module._get_square(g, row, col);
            text += square2str(s);
        }
        text += "\n";
    }

    // put this text in <div> element with ID 'result'
    var elm = document.getElementById('result');
    elm.innerHTML = text;
}

function start() {
    console.log("call start routine");
    g = Module._new_default();

    Module._play_move(g, 1, 0, TENT);
    printGame(g);
}

// Demo Canvas (JavaScript)

/* ******************** global variables ******************** */

var canvas = document.getElementById('mycanvas');

var imgTent = new Image();
imgTent.src = "tent.png";
var imgTree = new Image();
imgTree.src = "tree.png";
var imgBigTree = new Image();
imgBigTree.src = "bigtree.png";
var imgGrass = new Image();
imgGrass.src = "grass.png";


/* ******************** register events ******************** */

window.addEventListener('load', windowLoad);              // window load
canvas.addEventListener('click', canvasLeftClick);        // left click event
canvas.addEventListener('contextmenu', canvasRightClick); // right click event
document.getElementById('player').addEventListener('click', function () {
    drawCanvas();
})
document.getElementById('btn_fill_grass').addEventListener('click', function () {
    Module._fill_grass(g);
    drawCanvas();
})
document.getElementById('btn_undo').addEventListener('click', function () {
    Module._undo(g);
    drawCanvas();
})
document.getElementById('btn_redo').addEventListener('click', function () {
    Module._redo(g);
    drawCanvas();
})
document.getElementById('btn_restart').addEventListener('click', function () {
    Module._restart(g);
    drawCanvas();
})

/* ******************** event callback ******************** */

function canvasLeftClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    if (!g) return;
    console.log("left click at position:", event.offsetX, event.offsetY);
    let col = Math.floor(event.offsetX / taille_case); // on convertit l'offset en pixel pour les cols
    let row = Math.floor(event.offsetY / taille_case); // on convertit l'offset en pixel pour les rows

    // update position of tents / grass
    let current = Module._get_square(g, row, col);
    if (current == 0 || current == 3) {
        Module._play_move(g, row, col, TENT);
    }
    else if (current == 2) {
        Module._play_move(g, row, col, EMPTY);
    }
    drawCanvas();
}

function canvasRightClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    if (!g) return;
    console.log("right click at position:", event.offsetX, event.offsetY);
    let col = Math.floor(event.offsetX / taille_case); // on convertit l'offset en pixel pour les cols
    let row = Math.floor(event.offsetY / taille_case); // on convertit l'offset en pixel pour les rows

    // update position of mario image used by drawCanvas()
    let current = Module._get_square(g, row, col);
    if (current == 0 || current == 2) {
        Module._play_move(g, row, col, GRASS);
    }
    else if (current == 3) {
        Module._play_move(g, row, col, EMPTY);
    }
    drawCanvas();
}

function windowLoad() {
    console.log("window load");
    drawCanvas();
}

/* ******************** canvas drawing ******************** */

function drawCanvas() {
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    // clear canvas
    ctx.clearRect(0, 0, width, height);

    // draw a yellow square at (squareX, squareY) position

    // draw some lines
    //ctx.save();
    //ctx.strokeStyle = 'blue';
    //ctx.moveTo(0, 0);
    //ctx.lineTo(width, height);
    //ctx.moveTo(width, 0);
    //ctx.lineTo(0, height);
    //ctx.stroke(); // the stroke will actually paint the current lines
    //ctx.restore();

    ctx.save();
    ctx.strokeStyle = 'black';

    // je commence un nouveau dessin pour supprimer les lignes si il y a chnagement
    ctx.beginPath();

    let size_game_col = Math.max(2, Math.min(25, parseInt(document.getElementById('input_nb_cols').value)));
    let size_game_row = Math.max(2, Math.min(25, parseInt(document.getElementById('input_nb_rows').value)));
    taille_case = Math.min(canvas.width / size_game_col, canvas.height / size_game_row);
    let x = 0;

    // background du jeu
    ctx.fillStyle = 'red';
    ctx.fillRect(0, 0, size_game_col * taille_case, size_game_row * taille_case);

    for (let i = 0; i <= size_game_col; i++) {
        ctx.moveTo(x, 0);
        ctx.lineTo(x, size_game_row * taille_case);
        x = x + taille_case;
    }
    x = 0;
    for (let i = 0; i <= size_game_row; i++) {
        ctx.moveTo(0, x);
        ctx.lineTo(size_game_col * taille_case, x);
        x = x + taille_case;
    }
    ctx.stroke();
    ctx.restore();

    let current;
    for (let i = 0; i < size_game_row; i++) {
        for (let j = 0; j < size_game_col; j++) {
            current = Module._get_square(g, i, j);
            switch (current) {
                case TENT:
                    ctx.drawImage(imgTent, j * taille_case, i * taille_case, taille_case, taille_case);
                    break;
                case GRASS:
                    ctx.drawImage(imgGrass, j * taille_case, i * taille_case, taille_case, taille_case);
                    break;
                case BIGTREE:
                    ctx.drawImage(imgBigTree, j * taille_case, i * taille_case, taille_case, taille_case);
                    break;
                case TREE:
                    ctx.drawImage(imgTree, j * taille_case, i * taille_case, taille_case, taille_case);
                    break;
            }
        }
    }

    // draw a centered text
    //ctx.save();
    //ctx.font = 'bold 20px Arial';
    //ctx.fillStyle = 'black';
    //ctx.textBaseline = 'middle';
    //ctx.textAlign = 'center';
    //ctx.fillText("Hello World!", width / 2, height / 2);
    //ctx.restore();

    // draw mario image at (marioX, marioY) position
    //ctx.drawImage(mario, marioX, marioY, 80, 80);

    //ctx.drawImage(brr, brrX, brrY, 80, 80);
}

// EOF
