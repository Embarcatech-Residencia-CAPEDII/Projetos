
import { userLogin } from "./api.js";

document.getElementById("loginForm").addEventListener("submit", async function (event) {
    event.preventDefault(); // Impede o envio tradicional do formulário

    const email = document.getElementById("email").value;
    const senha = document.getElementById("senha").value;

    try {
        const result = await userLogin(email, senha);
        console.log("Usuário cadastrado:", result);
    } catch (err) {
        console.error("Erro:", err);
    }
});