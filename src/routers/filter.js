const express = require('express');
const path = require('path');
const multer = require('multer');
const uuid = require('uuid').v4;
const {
    filter,
    convertFileToPPM,
    convertToOriginalFileType}  = require('../utils/filtro.js');
const {
    userFilesLookup,
    generateNewUser,
    cleanUserArray,
    validatorMiddleware,
    doesUserHaveValidID} = require('../utils/user')

const router = new express.Router();

router.use(express.urlencoded({
  extended: true
}));

const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null,path.join(__dirname, '../../uploads/'));
    },
    filename: (req, file, cb) => {
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
})

router.post('/filter/result', upload.single('input_img'), validatorMiddleware, async (req, res) => {
    const out = convertFileToPPM(req.file.filename, 'out-' + req.file.filename + '.ppm', () => {
        filter('out-' + req.file.filename + '.ppm', req.body.filter, req.body.filter_option, req.file.filename + '.ppm', () => {
            convertToOriginalFileType(req.file.filename + '.ppm', req.file.filename, () => {
                res.render('result', {
                    title: 'Result',
                    image: req.file.filename
                })
            });
        });
    });
})

const filterRouter = router;

module.exports = {
    filterRouter
};
