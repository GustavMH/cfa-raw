import torch
from   torch import nn

class DenoisingUnet(nn.Module):
    def __init__(self, channels=3, ks=64):
        super(DenoisingUnet, self).__init__()
        # Encoder
        self.enc1 = nn.Sequential(
            nn.Conv2d(channels, ks, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks),
            nn.ReLU(True))
        self.enc2 = nn.Sequential(
            nn.Conv2d(ks, ks*2, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2),
            nn.ReLU(True))
        self.enc3 = nn.Sequential(
            nn.Conv2d(ks*2, ks*2**2, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2**2),
            nn.ReLU(True))
        self.enc4 = nn.Sequential(
            nn.Conv2d(ks*2**2, ks*2**3, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2**3),
            nn.ReLU(True))
        self.enc5 = nn.Sequential(
            nn.Conv2d(ks*2**3, ks*2**3, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2**3),
            nn.ReLU(True))

        # Decoder
        self.dec1 = nn.Sequential(
            nn.ConvTranspose2d(ks*2**3, ks*2**3, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2**3),
            nn.ReLU(True),
            nn.Dropout(0.5))
        self.dec2 = nn.Sequential(
            nn.ConvTranspose2d(2*ks*2**3, ks*2**2, kernel_size=4, stride=2, padding=1), # the 1024 comes from concatenation
            nn.BatchNorm2d(ks*2**2),
            nn.ReLU(True))
        self.dec3 = nn.Sequential(
            nn.ConvTranspose2d(2*ks*2**2, ks*2**1, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2**1),
            nn.ReLU(True))
        self.dec4 = nn.Sequential(
            nn.ConvTranspose2d(2*ks*2**1, ks*2**0, kernel_size=4, stride=2, padding=1),
            nn.BatchNorm2d(ks*2**0),
            nn.ReLU(True))
        self.dec5 = nn.Sequential(
            nn.ConvTranspose2d(2*ks*2**0, channels, kernel_size=4, stride=2, padding=1),
            nn.Tanh())

    def forward(self, x):
        # Encoder
        e1 = self.enc1(x)
        e2 = self.enc2(e1)
        e3 = self.enc3(e2)
        e4 = self.enc4(e3)
        e5 = self.enc5(e4)

        # Decoder with skip connections
        d1 = self.dec1(e5)
        d1 = torch.cat((d1, e4), dim=1)  # skip connection
        d2 = self.dec2(d1)
        d2 = torch.cat((d2, e3), dim=1)  # skip connection
        d3 = self.dec3(d2)
        d3 = torch.cat((d3, e2), dim=1)  # skip connection
        d4 = self.dec4(d3)
        d4 = torch.cat((d4, e1), dim=1)  # skip connection
        d5 = self.dec5(d4)
        return d5


if __name__ == "__main__":
    model = DenoisingUnet()
    print(sum(p.numel() for p in model.parameters() if p.requires_grad))
