const abrir = document.getElementById('abrir');
const fechar = document.getElementById('fechar');
const form = document.getElementById('formulario');
const formUsers = document.getElementById('formUser');

abrir.addEventListener('click', () => {
  form.style.display = 'block';
});

fechar.addEventListener('click', () => {
  form.style.display = 'none';
});

formUsers.addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData(formUsers);
  const dados = Object.fromEntries(formData.entries());

  try {
    const resposta = await fetch('/api/users/register', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'  
      },
      body: JSON.stringify(dados)
    });

    const resultado = await resposta.json();
    console.log(resultado);
    carregarUsuarios();

  } catch (error) {
    console.error(error);
  }
});

async function carregarUsuarios() {
  const resposta = await fetch('/api/users');  // pega JSON do Flask
  const usuarios = await resposta.json();

  const planilha = document.getElementById('userTable');

  planilha.innerHTML = "";

  usuarios.forEach(usuario => {
    const linha = document.createElement('div');
    linha.classList.add('linha');

    linha.innerHTML += `
      <div class="celula">0</div>
      <div class="celula">${usuario.username}</div>
      <div class="celula">${usuario.role}</div>
      <div class="celula">
        <button class="btn-excluir" data-id="${usuario._id.$oid}">Excluir</button>
        <button class="btn-editar" data-id="${usuario._id.$oid}">Alterar</button>
      </div>
    `

    planilha.appendChild(linha);
    })

  document.querySelectorAll('.btn-excluir').forEach(btn => {
    btn.addEventListener('click', async function() {
      const id = this.getAttribute('data-id');
      const resposta = await fetch(`/usuarios/excluir/${id}`, { method: 'DELETE' });
      if (resposta.ok) {
        alert(`Usuário ${id} excluído`);
        location.reload(); 
      } else {
        alert(`Erro ao excluir o usuário ${id}`);
      }
    });
  });
}

carregarUsuarios();
