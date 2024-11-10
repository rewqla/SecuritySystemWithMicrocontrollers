const express = require('express');
const path = require('path');
const app = express();
const port = 3000;

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({ extended: true }));

const users = [
    { username: 'admin', password: '1234' }
];

app.get('/', (req, res) => {
    res.render('login', { messages: [] });
});
app.get('/register', (req, res) => {
    res.render('register', { messages: [] });
});

app.get('/cabinet', (req, res) => {
    res.render('cabinet');
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;

    const user = users.find(user => user.username === username && user.password === password);
    if (user) {
        res.redirect('/cabinet');
    } else {
        res.render('login', { messages: ['Invalid credentials'] });
    }
});

app.post('/register', (req, res) => {
    const { username, password } = req.body;

    const existingUser = users.find(user => user.username === username);
    if (existingUser) {
        res.render('register', { messages: ['Username already exists. Please choose another one.'] });
    }

    users.push({ username, password });
    res.render('login', { messages: ['Registration successful! You can now log in.'] });
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
