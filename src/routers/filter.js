const express = require('express');
const filter = require('../utils/filtro.js');
const path = require('path');
const multer = require('multer');
const uuid = require('uuid').v4;

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
        cb(null, req.body.userID + '-' + file.originalname);
    }
})

const upload = multer({
    storage,
    limits: {
        fileSize: 10000000
    },
    // fileFilter(req, file, cb){
    //     cb(new Error("File must be either JPG, PNG or PNM"))
    // }
})

const middleware = (req, res, next) => {
    console.log("Middleware body", req.body)
    next();
}

router.get('/filter/upload', (req, res) => {
    res.render('upload', {
        title: 'Upload Image',
        name: 'Juanjo'
    })
    //filter.filter("", "")
})

router.post('/filter/result', upload.single('input_img'), (req, res) => {
    console.log("File name on server ", req.file.filename);
    console.log("File path on server ", req.file.path);
    console.log("Original name ", req.file.originalname);
    console.log("Filter options", req.body);

    res.render('result', {
        title: 'Result'
    })
})

module.exports = router;
