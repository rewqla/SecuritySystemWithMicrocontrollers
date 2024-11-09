import { renderToReadableStream } from "react-dom/server";
import Layout from "./layout";

// Define the Component first
function Component(props) {
  return (
    <div>
      <h1>{props.message}</h1>
    </div>
  );
}

// Define HomePage
function HomePage() {
  return (
    <Layout>
      <main>
        <h2>Welcome to My Bun App</h2>
        <p>This is a simple page served by Bun with a custom layout.</p>
      </main>
    </Layout>
  );
}

Bun.serve({
  async fetch() {
    // Render the Component to a stream
    const stream = await renderToReadableStream(
      <Component message="Hello from server!" />
    );
    // Return the response with the stream as the body
    return new Response(stream, {
      headers: { "Content-Type": "text/html" },
    });
  },
});
