def save_losses(losses, loss_dest):
    with open(loss_dest, "w") as f:
        f.writelines([f"{loss}\n" for loss in losses])

def validation_imgs(model, inputs_clean, inputs_noise, savepath=None):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    with torch.no_grad():
        outputs = model(inputs_noise[:3].to(device))

        fig, axs = plt.subplots(3, 3, figsize=(15, 6))
        for i in range(3):
            for n, (title, data) in enumerate([
                ("Target image", inputs_clean[:3]),
                ("Input noisy",  inputs_noise[:3]),
                ("Result image", outputs)
            ]):
                axs[n, i].imshow(data[i].permute(1, 2, 0).squeeze())
                axs[n, i].title.set_text(title + str(i+1))
                axs[n, i].axis('off')

        if savepath:
            plt.savefig(savepath)
        else:
            plt.show()


def validate(model, val_clean, val_noise):
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

    paired_dataset = PairedDataset(val_clean, val_noise)
    paired_loader  = DataLoader(paired_dataset, batch_size=32, shuffle=False)

    model.eval()
    model.to(device)

    def loss(inputs_clean, inputs_noise):
        outputs = model(inputs_noise.to(device))
        mse = F.mse_loss(outputs, inputs_clean.to(device))
        return mse

    with torch.no_grad():
        return [loss(*inputs) for inputs in paired_loader]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        prog="train.py",
        description="Trains a denoising model"
    )
    parser.add_argument("--clean", required=True, help="Path to the clean data")
    parser.add_argument("--noise", required=True, help="Path to the noisy data")
    parser.add_argument("--output", required=True, help="Path to the output folder, model and loss are saved here")
    parser.add_argument("--name", required=True, help="Name prefix for the output files")
    parser.add_argument("--type", required=True, help="Noise input file type")
    parser.add_argument("--model", required=True, help="Path to model .pkl")

    args = parser.parse_args()

    print("DEVICE:", torch.device("cuda:0" if torch.cuda.is_available() else "cpu"))

    val_clean = load_images(Path(args.clean) / "val", t=".png")
    val_noise = load_images(Path(args.noise) / "val", t=args.type)

    model = pickle.load(open(Path(args.model)))

    losses = validate(model, val_clean, val_noise)
    save_losses(losses, loss_dest=Path(args.output) / f"{args.name}-val-loss.txt")
    validate_imgs(model, val_clean, val_noise, savepath=Path(args.output) / f"{args.name}-imgs.png")
