export default function Layout({ children }) {
  return (
    <html lang="en">
      <head>
        <meta charSet="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <title>My Bun App</title>
        <style>
          {`
            body { font-family: Arial, sans-serif; margin: 0; padding: 0; }
            header, footer { background-color: #333; color: white; padding: 1em; text-align: center; }
            nav a { color: white; margin: 0 10px; text-decoration: none; }
            .container { max-width: 800px; margin: 20px auto; padding: 20px; }
          `}
        </style>
      </head>
      <body>
        <header>
          <h1>My Bun App</h1>
          <nav>
            <a href="/">Home</a>
            <a href="/about">About</a>
          </nav>
        </header>
        <div className="container">{children}</div>
        <footer>
          <p>&copy; {new Date().getFullYear()} My Bun App</p>
        </footer>
      </body>
    </html>
  );
}
