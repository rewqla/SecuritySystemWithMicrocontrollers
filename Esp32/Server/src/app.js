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
    res.render('login');
});
app.get('/register', (req, res) => {
    res.render('register');
});

app.get('/cabinet', (req, res) => {
    res.render('cabinet');
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;
    console.log(users)
    const user = users.find(user => user.username === username && user.password === password);
    if (user) {
        res.redirect('/cabinet');
    } else {
        res.send('Invalid credentials');
    }
});

app.post('/register', (req, res) => {
    const { username, password } = req.body;

    const existingUser = users.find(user => user.username === username);
    if (existingUser) {
        return res.send('Username already exists. Please choose another one.');
    }

    users.push({ username, password });
    res.send('Registration successful! You can now <a href="/">login</a>.');
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
