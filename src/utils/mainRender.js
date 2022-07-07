const {generateNewUser} = require('./user')

const mainRender = (res) => {
    const sessionID = generateNewUser();
    res.render('index', {
        title: 'Main',
        name: 'Juanjo',
        sessionID
    })
}

module.exports = mainRender;
