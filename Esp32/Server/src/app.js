const express = require('express');
const path = require('path');
const app = express();
const port = 3000;

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));

app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
    res.render('login');
});

app.get('/cabinet', (req, res) => {
    res.render('cabinet');
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;

    if (username === 'admin' && password === 'password') {
        res.redirect('/cabinet');
    } else {
        res.send('Invalid credentials');
    }
});

app.get('/api/configuration', (req, res) => {
    const configuration = {
        text: "config text"
    };

    res.json(configuration);
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
