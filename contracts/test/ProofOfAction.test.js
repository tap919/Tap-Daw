const { expect } = require("chai");
const { ethers } = require("hardhat");

describe("ProofOfAction", function () {
  let proofOfAction;
  let owner;
  let addr1;

  beforeEach(async function () {
    [owner, addr1] = await ethers.getSigners();
    const ProofOfAction = await ethers.getContractFactory("ProofOfAction");
    proofOfAction = await ProofOfAction.deploy();
  });

  describe("Attestation", function () {
    it("Should attest an action with LEARN phase", async function () {
      const action = "Completed tutorial";
      const phase = 0; // LEARN

      await expect(proofOfAction.attest(action, phase))
        .to.emit(proofOfAction, "Attested")
        .withArgs(owner.address, action, phase, await getBlockTimestamp());
    });

    it("Should attest an action with SHIP phase", async function () {
      const action = "Deployed feature";
      const phase = 1; // SHIP

      await expect(proofOfAction.attest(action, phase))
        .to.emit(proofOfAction, "Attested")
        .withArgs(owner.address, action, phase, await getBlockTimestamp());
    });

    it("Should attest an action with REST phase", async function () {
      const action = "Taking break";
      const phase = 2; // REST

      await expect(proofOfAction.attest(action, phase))
        .to.emit(proofOfAction, "Attested")
        .withArgs(owner.address, action, phase, await getBlockTimestamp());
    });

    it("Should increment attestation count", async function () {
      await proofOfAction.attest("Action 1", 0);
      expect(await proofOfAction.getAttestationCount(owner.address)).to.equal(1);

      await proofOfAction.attest("Action 2", 1);
      expect(await proofOfAction.getAttestationCount(owner.address)).to.equal(2);
    });

    it("Should revert if action is empty", async function () {
      await expect(proofOfAction.attest("", 0))
        .to.be.revertedWith("Action cannot be empty");
    });

    it("Should track attestations separately for different addresses", async function () {
      await proofOfAction.connect(owner).attest("Owner action", 0);
      await proofOfAction.connect(addr1).attest("Addr1 action", 1);

      expect(await proofOfAction.getAttestationCount(owner.address)).to.equal(1);
      expect(await proofOfAction.getAttestationCount(addr1.address)).to.equal(1);
    });

    it("Should emit phase in Attested event", async function () {
      const tx = await proofOfAction.attest("Test action", 1);
      const receipt = await tx.wait();
      
      const event = receipt.logs.find(log => {
        try {
          const parsed = proofOfAction.interface.parseLog(log);
          return parsed.name === "Attested";
        } catch {
          return false;
        }
      });
      
      expect(event).to.not.be.undefined;
      const parsedEvent = proofOfAction.interface.parseLog(event);
      expect(parsedEvent.args.phase).to.equal(1); // SHIP phase
    });
  });

  async function getBlockTimestamp() {
    const blockNumber = await ethers.provider.getBlockNumber();
    const block = await ethers.provider.getBlock(blockNumber);
    return block.timestamp + 1; // Next block timestamp
  }
});
