const express = require('express');
const path = require('path');
const hbs = require('hbs');
const {
    filterRouter,
    userFilesLookup} = require('./routers/filter');
const {mainRender,
    cleanUserArray} = require('./utils/user');



//Load Express and set port
const app = express();
const port = process.env.PORT || 3000;

//Configure handlebars
app.set('view engine', 'hbs');
app.set('views', path.join(__dirname, '../templates/views'));
hbs.registerPartials(path.join(__dirname, '../templates/partials'))

//Set routers
app.use(filterRouter)

//Set static files directory
app.use(express.static(path.join(__dirname, '../public')))

//Clean userFilesLookup periodically
// setInterval(cleanUserArray, 10000);

app.get('', (req, res) => {
    mainRender(res);
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
