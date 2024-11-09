// app.js
const express = require('express');
const path = require('path');
const app = express();
const port = 3000;

// Set EJS as the templating engine
app.set('view engine', 'ejs');

// Serve static files (like CSS) from the 'public' directory
app.use(express.static(path.join(__dirname, 'public')));

// Middleware to handle POST data
app.use(express.urlencoded({ extended: true }));

// Route to render login page
app.get('/', (req, res) => {
    res.render('login');
});

// Route to handle form submission
app.post('/login', (req, res) => {
    const { username, password } = req.body;

    // Simple authentication logic (you can replace this with real authentication)
    if (username === 'admin' && password === 'password') {
        res.send('Login successful');
    } else {
        res.send('Invalid credentials');
    }
});

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
