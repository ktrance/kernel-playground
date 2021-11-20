Vagrant.configure("2") do |config|
    if defined?(VagrantVbguest::Middleware)
        config.vbguest.auto_update = true
    end
        
    config.vm.box = "bento/ubuntu-20.04"
    config.vm.box_check_update = false

    config.vm.network "public_network", bridge: "enp3s0"

    config.vm.provider "virtualbox" do |vb|
        vb.memory = 4096
        vb.cpus = 2
    end

    config.vm.provision "shell", path: 'setup.sh'
end