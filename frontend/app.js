const statusNode = document.getElementById('status');
const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');

function setStatus(msg) {
  statusNode.textContent = msg;
}

function bindController(controller) {
  controller.imageUpdated.connect((dataUrl, width, height) => {
    const img = new Image();
    img.onload = () => {
      canvas.width = width;
      canvas.height = height;
      ctx.drawImage(img, 0, 0);
      setStatus(`图像尺寸: ${width}x${height}`);
    };
    img.src = dataUrl;
  });

  controller.statusMessage.connect((msg) => setStatus(msg));

  document.getElementById('btnOpen').onclick = () => {
    const path = document.getElementById('inputPath').value.trim();
    controller.openImage(path);
  };

  document.getElementById('btnSave').onclick = () => {
    const path = document.getElementById('inputPath').value.trim();
    controller.saveImage(path);
  };

  document.getElementById('btnRotate').onclick = () => controller.rotate90Clockwise();
  document.getElementById('btnExposure').onclick = () => {
    const factor = parseFloat(document.getElementById('exposure').value || '1');
    controller.adjustExposure(factor);
  };

  document.getElementById('btnCrop').onclick = () => {
    controller.crop(
      parseInt(document.getElementById('cropX').value || '0', 10),
      parseInt(document.getElementById('cropY').value || '0', 10),
      parseInt(document.getElementById('cropW').value || '100', 10),
      parseInt(document.getElementById('cropH').value || '100', 10)
    );
  };

  async function refreshModules() {
    const modules = await controller.listModules();
    const select = document.getElementById('moduleSelect');
    select.innerHTML = '';
    modules.forEach((name) => {
      const option = document.createElement('option');
      option.value = name;
      option.textContent = name;
      select.appendChild(option);
    });
  }

  document.getElementById('btnRefreshModules').onclick = refreshModules;
  document.getElementById('btnApplyModule').onclick = () => {
    const selected = document.getElementById('moduleSelect').value;
    controller.applyModule(selected);
  };

  refreshModules();
}

new QWebChannel(qt.webChannelTransport, (channel) => {
  bindController(channel.objects.imageController);
});
