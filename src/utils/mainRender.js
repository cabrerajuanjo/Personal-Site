const {generateNewUser} = require('./user')

//Renders Main Page either when first acces page or when needs to get UUID (when is deleted due to time)
const mainRender = (res) => {
    const sessionID = generateNewUser();
    res.render('index', {
        title: 'Main',
        name: 'Juanjo',
        sessionID
    })
}

module.exports = mainRender;
