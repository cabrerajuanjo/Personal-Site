const express = require('express');
const path = require('path');
const hbs = require('hbs');
const uuid = require('uuid').v4;
const filterRouter = require('./routers/filter');

//Load Express and set port
const app = express();
const port = process.env.PORT || 3001;

//Configure handlebars
app.set('view engine', 'hbs');
app.set('views', path.join(__dirname, '../templates/views'));
hbs.registerPartials(path.join(__dirname, '../templates/partials'))

//Set routers
app.use(filterRouter)

//Set static files directory
app.use(express.static(path.join(__dirname, '../public')))


app.get('', (req, res) => {
    sessionID = (uuid());
    console.log(sessionID);
    res.render('index', {
        title: 'Main',
        name: 'Juanjo',
        sessionID
    })


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
    console.log('listening on ', port)
})
