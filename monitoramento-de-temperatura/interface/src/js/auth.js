
document.getElementById("loginForm").addEventListener("submit", async function(event) {
    event.preventDefault(); // Impede o envio tradicional do formulário

    const usuario = document.getElementById("usuario").value;
    const senha = document.getElementById("senha").value;

    // Prepara o corpo da requisição com as credenciais
    const body = {
        email: usuario,
        password: senha
    };

    console.log(body);

    try {
        // Faz a requisição de login
        const response = await fetch("http://localhost:3000/api/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(body),
        });

        const data = await response.json();
        console.log(data);

        if (response.ok) {
            // Se o login for bem-sucedido, redireciona o usuário
            // Aqui você pode guardar o token no localStorage ou sessionStorage
            localStorage.setItem("token", data.token); // Exemplo de como salvar o token

            // Redireciona para a página de dashboard ou home após login
            window.location.href = "index.html"; // Substitua com sua URL de destino
        } else {
            // Se o login falhar, exibe a mensagem de erro
            alert(data.message || "Erro no login.");
        }
    } catch (error) {
        console.error("Erro ao fazer login:", error);
        alert("Erro ao tentar fazer login. Tente novamente.");
        console.log(data.token);
    }
});