const express = require('express');
const path = require('path');
const fs = require('fs');
const hbs = require('hbs');
const multer = requi= require('multer');
//const formidable = require('formidable');
//const upload = require('express-fileupload');
const cors = require('cors');
const uuid = require('uuid').v4;

const filter = require('./utils/filtro.js');

const app = express();
const port = process.env.PORT || 3000;
const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null,path.join(__dirname, '../uploads/'));
    },
    filename: (req, file, cb) => {
        const {originalname} = file;
        cb(null, uuid() + '-' + originalname);
    }
})
const upload = multer({storage})

const viewsPath = path.join(__dirname, '../templates/views');
const partialsPath = path.join(__dirname, '../templates/partials')

app.set('view engine', 'hbs');
app.set('views', viewsPath);

hbs.registerPartials(partialsPath)

app.use(cors())
app.use(express.static(path.join(__dirname, '../public')))


app.get('', (req, res) => {
    res.render('index', {
        title: 'Main',
        name: 'Juanjo'})
})

app.get('/filter/upload', (req, res) => {
    res.render('upload', {
        title: 'Upload Image',
        name: 'Juanjo'
    })
    //filter.filter("", "")
})

app.post('/filter/set-filter', upload.single('input_img'), (req, res) => {
    res.render('set-filter', {})

})

app.get('/about', (req, res) => {
    res.render('about', {
        title: 'About me',
        name: 'Juanjo'})
})

app.get('/help', (req, res) => {
    res.render('help', {
        message: 'This is help',
        title: 'Help',
        name: 'Juanjo'
    })
})


app.get('/help/*', (req, res) => {
    res.render('notfound', {
        title: 'Error 404',
        message: 'Help article',
        name: 'Juanjo'
    })
})

app.get('*', (req, res) => {
    res.render('notfound', {
        title: 'Error 404',
        message: 'Page',
        name: 'Juanjo'
})
})

app.listen(port, () => {
    console.log('listening on 3000')
})
