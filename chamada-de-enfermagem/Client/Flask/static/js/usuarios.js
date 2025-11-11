const abrir = document.getElementById('abrir');
const fechar = document.getElementById('fechar');
const fecharPut = document.getElementById('fecharPut');
const form = document.getElementById('formCadastro');
const formPut = document.getElementById('formPut');
const overlay = document.getElementById('overlay');

let idEditando = null;
let nomeEditando = null;

abrir.addEventListener('click', () => {
  form.style.display = 'block';
  overlay.style.display = 'block';
});

fechar.addEventListener('click', () => {
  overlay.style.display = 'none';
  form.style.display = 'none';
});

fecharPut.addEventListener('click', () => {
  overlay.style.display = 'none';
  formPut.style.display = 'none';
});

form.addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData(form);
  const dados = Object.fromEntries(formData.entries());

  console.log(nomeEditando)
  console.log(dados.username)

  

  try {
      // Modo adicionar 
      const resposta = await fetch('/api/users/register', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(dados)
      });

      if (resposta.ok) {
        form.style.display = 'none';
        overlay.style.display = 'none';
        carregarUsuarios();
      } else {
        alert('Erro ao adicionar usuário.');
    }
  } catch (error) {
    console.error(error);
  }
});

formPut.addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData(formPut);
  const dados = Object.fromEntries(formData.entries());

  dados["document_id"] = idEditando;

  if (dados.username === nomeEditando) {
    delete dados.username;
  }

  if (dados.password === "") {
    delete dados.password;
  }

  try {
      // Modo edição
      const resposta = await fetch(`/api/users/update`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(dados)
      });

      if (resposta.ok) {
        formPut.style.display = 'none';
        overlay.style.display = 'none';
        carregarUsuarios();
      } else {
        alert('Erro ao atualizar usuário.');
      } 
  } catch (error) {
    console.error(error);
  }
});

async function carregarUsuarios() {
  const resposta = await fetch('/api/users');  
  const usuarios = await resposta.json();

  const planilha = document.getElementById('userTable');

  planilha.innerHTML = "";

  usuarios.forEach(usuario => {
    const linha = document.createElement('div');
    linha.classList.add('linha');

    linha.innerHTML += `
      <div class="celula">${usuario.username}</div>
      <div class="celula">${usuario.role}</div>
      <div class="celula">
        <button class="btn-excluir" data-id="${usuario._id}">Excluir</button>
        <button class="btn-editar" data-id="${usuario._id}">Alterar</button>
      </div>
    `

    planilha.appendChild(linha);
    })

  document.querySelectorAll('.btn-excluir').forEach(btn => {
    btn.addEventListener('click', async function() {
      const id = this.getAttribute('data-id');
      const resposta = await fetch(`/api/users/delete/${id}`, { method: 'DELETE' });
      if (resposta.ok) {
        carregarUsuarios(); 
      } else {
        alert(`Erro ao excluir o usuário ${id}`);
      }
    });
  },

  document.querySelectorAll('.btn-editar').forEach(btn => {
    btn.addEventListener('click', function () {
      const id = this.getAttribute('data-id');
      const linha = this.closest('.linha');
      const nome = linha.querySelector('.celula:nth-child(1)').innerText;
      const funcao = linha.querySelector('.celula:nth-child(2)').innerText;

      // Preenche o formulário com os dados atuais
      document.querySelector('#formPut input[name="username"]').value = nome;
      // document.querySelector('#formPut input[name="role"]').value = funcao;

      // Marca que estamos editando
      idEditando = id;
      nomeEditando = nome;

      // Abre o formulário
      formPut.style.display = 'block';
      overlay.style.display = 'block';
    });
  })
);
}

async function putUsuarios(dados) {
  try {
    const resposta = await fetch('/api/users/register', {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json'  
      },
      body: JSON.stringify(dados)
    });
    carregarUsuarios();

  } catch (error) {
    console.error(error);
  }
}


carregarUsuarios();
