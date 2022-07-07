const express = require('express');
const filter = require('../utils/filtro.js');
const path = require('path');
const multer = require('multer');
const uuid = require('uuid').v4;
const {
    userFilesLookup,
    generateNewUser,
    cleanUserArray,
    validatorMiddleware,
    doesUserHaveValidID} = require('../utils/user')

const router = new express.Router();



// const formParse = multer();

router.use(express.urlencoded({
  extended: true
}));

const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null,path.join(__dirname, '../../uploads/'));
    },
    filename: (req, file, cb) => {
        const id = uuid()

        cb(null, uuid() + '-' + file.originalname);
    }
})

const upload = multer({
    storage,
    limits: {
        fileSize: 10000000
    },

})

router.get('/filter/upload', (req, res) => {
    console.log("FILTER", userFilesLookup)
    res.render('upload', {
        title: 'Upload Image',
        name: 'Juanjo'
    })
    //filter.filter("", "")
})

router.post('/filter/result', upload.single('input_img'), validatorMiddleware, (req, res) => {
    console.log("File name on server ", req.file.filename);
    console.log("File path on server ", req.file.path);
    console.log("Original name ", req.file.originalname);
    console.log("Filter options", req.body);

    res.render('result', {
        title: 'Result'
    })
})

const filterRouter = router;

module.exports = {
    filterRouter
};
